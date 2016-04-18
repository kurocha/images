//
//  JPEGImage.cpp
//  File file is part of the "vulkanraum" project and released under the Orion Transfer Commercial License.
//
//  Created by Samuel Williams on 18/4/2016.
//  Copyright, 2016, by Samuel Williams. All rights reserved.
//

#include "JPEGImage.hpp"

namespace Dream
{
	namespace Imaging
	{
		JPEGImage::JPEGImage()
		{
		}
		
		JPEGImage::~JPEGImage()
		{
		}
		
#if 0
		static void jpeg_dummy (j_decompress_ptr cinfo) {
		}

		static boolean jpeg_fill_input_buffer (j_decompress_ptr cinfo) {
			// Force the jpeg library to stop reading if we run past the end of data
			static JOCTET eoi[2] = {(JOCTET)0xff, (JOCTET)JPEG_EOI};
			cinfo->src->next_input_byte = eoi;
			cinfo->src->bytes_in_buffer = 2;

			return (boolean)0;
		}

		/*
		 * Skip data --- used to skip over a potentially large amount of
		 * uninteresting data (such as an APPn marker).
		 */

		static void jpeg_skip_input_data (j_decompress_ptr cinfo, long num_bytes) {
			//DREAM_ASSERT(num_bytes < cinfo->src.bytes_in_buffer && "jpeglib tried to skip further than the end of file!!");

			if (num_bytes > 0 && num_bytes < (long)cinfo->src->bytes_in_buffer) {
				cinfo->src->next_input_byte += (std::size_t) num_bytes;
				cinfo->src->bytes_in_buffer -= (std::size_t) num_bytes;
			} else {
				jpeg_fill_input_buffer(cinfo);
			}
		}

		static void jpeg_memory_src (j_decompress_ptr cinfo, const JOCTET * buffer, std::size_t bufsize) {
			if (cinfo->src == NULL) {
				cinfo->src = (jpeg_source_mgr *) (*cinfo->mem->alloc_small)((j_common_ptr)cinfo, JPOOL_PERMANENT, sizeof(jpeg_source_mgr));
			}

			cinfo->src->init_source = jpeg_dummy;

			cinfo->src->fill_input_buffer = jpeg_fill_input_buffer;
			cinfo->src->skip_input_data = jpeg_skip_input_data;

			cinfo->src->resync_to_restart = jpeg_resync_to_restart; // default
			cinfo->src->term_source = jpeg_dummy;

			cinfo->src->next_input_byte = buffer;
			cinfo->src->bytes_in_buffer = bufsize;
		}

		static Ref<Image> load_jpeg_image (const Ptr<IData> data) {
			jpeg_decompress_struct cinfo;
			jpeg_error_mgr jerr = {0};

			Ref<Image> result_image;
			DataType data_type = DataType::BYTE;
			
			Shared<Buffer> buffer = data->buffer();

			unsigned width, height;

			try {
				cinfo.err = jpeg_std_error(&jerr);

				jpeg_create_decompress(&cinfo);
				jpeg_memory_src(&cinfo, buffer->begin(), buffer->size());

				jpeg_read_header(&cinfo, TRUE);

				width = cinfo.image_width;
				height = cinfo.image_height;

				unsigned row_width = 0;
				PixelFormat pixel_format;
				
				if (cinfo.jpeg_color_space == JCS_GRAYSCALE) {
					row_width = width;
					pixel_format = PixelFormat::L;
				} else {
					row_width = 3 * width;
					pixel_format = PixelFormat::RGB;
				}

				result_image = new Image(Vec2u(width, height), pixel_format, data_type);

				Byte *line = result_image->data();
				jpeg_start_decompress(&cinfo);

				// read jpeg image
				while (cinfo.output_scanline < cinfo.output_height) {
					jpeg_read_scanlines(&cinfo, &line, 1);
					line += row_width;
				}

				jpeg_finish_decompress(&cinfo);
				jpeg_destroy_decompress(&cinfo);
			} catch (std::exception &e) {
			}

			return result_image;
		};
#endif
	}
}
