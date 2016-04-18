//
//  PNGImage.cpp
//  File file is part of the "vulkanraum" project and released under the Orion Transfer Commercial License.
//
//  Created by Samuel Williams on 18/4/2016.
//  Copyright, 2016, by Samuel Williams. All rights reserved.
//

#include "PNGImage.hpp"

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
				}
			}
			
			void process_data(png_bytep buffer, png_uint_32 length)
			{
				if (setjmp(png_jmpbuf(png_reader))) {
					throw std::runtime_error("libpng blew up!");
				}

				png_process_data(png_reader, png_info, buffer, length);
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
		
		void PNGImage::convert(PixelLayout2D _layout, Byte * data)
		{
			PNGRowReader reader;
			
			reader.rows = _layout.generate_row_pointers(data);
			
			reader.read(*_data->buffer(), _data->size());
		}
	}
}
