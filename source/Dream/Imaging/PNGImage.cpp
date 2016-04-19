//
//  PNGImage.cpp
//  File file is part of the "vulkanraum" project and released under the Orion Transfer Commercial License.
//
//  Created by Samuel Williams on 18/4/2016.
//  Copyright, 2016, by Samuel Williams. All rights reserved.
//

#include "PNGImage.hpp"

extern "C" {
#include <png.h>
}

namespace Dream
{
	namespace Imaging
	{
		static void png_error (png_structp png_reader, png_const_charp msg) {
			throw std::runtime_error(msg);
		}
		
		struct PNGProgressiveReader {
			// internally used by libpng
			png_structp png_reader = nullptr;
			
			// user requested transforms
			png_infop png_info = nullptr;
			
			png_uint_32 width = 0, height = 0;
			int bit_depth, color_type, interlace_method, compression_method, filter_method;
			
			void extract_header() {
				png_get_IHDR(png_reader, png_info, &width, &height, &bit_depth, &color_type, &interlace_method, &compression_method, &filter_method);
			}
			
			bool finished = false, stop = false;
			
			std::vector<Byte*> rows;
			
			void read(const Buffer & buffer, std::size_t block_size) {
				std::size_t offset = 0;
				
				while (!stop) {
					std::size_t size = block_size;
					
					if (offset + size > buffer.size()) {
						size = buffer.size() - offset;
						stop = true;
					}
					
					process_data(buffer.begin() + offset, size);
					offset += size;
				}
			}
			
			void process_data(const Byte * data, std::size_t size)
			{
				log_debug("process_data(", (void*)data, ",", size, ")");
				
				if (setjmp(png_jmpbuf(png_reader))) {
					throw std::runtime_error("libpng blew up!");
				}

				png_process_data(png_reader, png_info, const_cast<png_bytep>(data), size);
			}
			
			static void info_callback(png_structp png_ptr, png_infop info) {
				png_start_read_image(png_ptr);
			}
			
			static void row_callback(png_structp png_ptr, png_bytep new_row, png_uint_32 row_num, int pass)
			{
				PNGProgressiveReader * reader = static_cast<PNGProgressiveReader *>(png_get_progressive_ptr(png_ptr));
				
				png_progressive_combine_row(png_ptr, reader->rows.at(row_num), new_row);
			}

			static void end_callback(png_structp png_ptr, png_infop info)
			{
				PNGProgressiveReader * reader = static_cast<PNGProgressiveReader *>(png_get_progressive_ptr(png_ptr));
				
				reader->finished = true;
			}
			
			PNGProgressiveReader() {
				png_reader = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, png_error, NULL);
				DREAM_ASSERT(png_reader != NULL && "png_create_read_struct returned NULL!");

				png_info = png_create_info_struct(png_reader);
				DREAM_ASSERT(png_info != NULL && "png_create_info_struct returned NULL!");
			}
			
			~PNGProgressiveReader() {
				png_destroy_read_struct(&png_reader, &png_info, (png_infopp)NULL);
			}
		};
		
		struct PNGHeaderReader : public PNGProgressiveReader {
			static void info_callback(png_structp png_ptr, png_infop info) {
				PNGHeaderReader * reader = static_cast<PNGHeaderReader *>(png_get_progressive_ptr(png_ptr));
				
				reader->extract_header();
				reader->stop = true;
				
				png_start_read_image(png_ptr);
			}
			
			PNGHeaderReader() {
				png_set_progressive_read_fn(png_reader, (void *)this, info_callback, NULL, NULL);
			}
		};
		
		struct PNGRowReader : public PNGProgressiveReader {
			static void prepare_for_rgba8(png_structp png_ptr, png_infop info) {
				PNGRowReader * reader = static_cast<PNGRowReader *>(png_get_progressive_ptr(png_ptr));
				
				reader->extract_header();
				
				if (reader->bit_depth < 8) {
					png_set_packing(png_ptr);
				} else if (reader->bit_depth > 8) {
					png_set_strip_16(png_ptr);
				}

				switch (reader->color_type) {
					case PNG_COLOR_TYPE_RGB:
						png_set_add_alpha(png_ptr, 0xFF, PNG_FILLER_AFTER);
					case PNG_COLOR_TYPE_RGB_ALPHA:
						break;
					default:
						png_set_expand(png_ptr);
				}
				
				png_start_read_image(png_ptr);
			}
			
			PNGRowReader() {
				png_set_progressive_read_fn(png_reader, (void *)static_cast<PNGProgressiveReader *>(this), prepare_for_rgba8, row_callback, end_callback);
			}
		};
		
		PNGImage::PNGImage(Ptr<IData> data) : Image(data)
		{
			DREAM_ASSERT(png_check_sig((png_byte*)data->buffer()->begin(), 8));
			
			PNGHeaderReader reader;
			reader.read(*data->buffer(), 256);
			_size = {reader.width, reader.height, 1};
		}
		
		PNGImage::~PNGImage()
		{
		}
		
		void PNGImage::convert(PixelLayout2D _layout, Byte * data) const
		{
			PNGRowReader reader;
			
			reader.rows = _layout.generate_row_pointers(data);
			
			reader.read(*_data->buffer(), _data->size());
		}
		
		static void png_write_to_buffer (png_structp png_writer, png_bytep data, png_size_t length)
		{
			DynamicBuffer * buffer = (DynamicBuffer*)png_get_io_ptr(png_writer);

			buffer->append(length, data);
		}

		Ref<IData> PNGImage::save(PixelLayout2D layout, const Byte * data)
		{
			// Structures to process the image:
			png_structp png_writer = NULL;
			png_infop png_info = NULL;
			
			// The buffer to contain the results:
			Shared<DynamicBuffer> result_data = new DynamicBuffer;
			
			// Data pointers, one per row:
			auto rows = layout.generate_row_pointers(data);
			
			try {
				png_writer = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, png_error, NULL);
				DREAM_ASSERT(png_writer != NULL && "png_create_write_struct returned NULL!");

				png_info = png_create_info_struct(png_writer);
				DREAM_ASSERT(png_info != NULL && "png_create_info_struct returned NULL!");

				png_set_write_fn(png_writer, static_cast<void *>(result_data.get()), png_write_to_buffer, NULL);

				int bit_depth = 8;
				int color_type = PNG_COLOR_TYPE_RGBA;
				
				auto size = layout.size();
				png_set_IHDR(png_writer, png_info, size[WIDTH], size[HEIGHT], bit_depth, color_type, 0, 0, 0);

				png_write_info(png_writer, png_info);

				png_write_image(png_writer, (png_bytepp)rows.data());

				// After you are finished writing the image, you should finish writing the file.
				png_write_end(png_writer, NULL);
			} catch (std::exception & e) {
				log_error("PNG write error: ", e.what());

				if (png_writer)
					png_destroy_write_struct(&png_writer, &png_info);

				throw;
			}

			png_destroy_write_struct(&png_writer, &png_info);

			return new BufferedData(result_data);
		}
	}
}
