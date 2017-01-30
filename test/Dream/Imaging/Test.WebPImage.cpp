//
//  Test.WebPImage.cpp
//  This file is part of the "Dream Imaging" project and released under the MIT License.
//
//  Created by Samuel Williams on 30/1/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include <UnitTest/UnitTest.hpp>

#include <Dream/Imaging/WebPImage.hpp>

namespace Dream
{
	namespace Imaging
	{
		UnitTest::Suite WebPImageTestSuite {
			"Dream::Imaging::WebPImage",
			
			
			{"can load file and size is correct",
				[](UnitTest::Examiner & examiner) {
					Ref<IData> data = new Core::LocalFileData("test/Dream/Imaging/test.webp");
					Ref<WebPImage> image = new WebPImage(data);
					
					examiner << "Width and size should be loaded correctly" << std::endl;
					examiner.expect(image->size()) == Vec3u{320, 241, 1};
				}
			},
			
			// {"can load file and convert to pixel buffer",
			// 	[](UnitTest::Examiner & examiner) {
			// 		Ref<IData> data = new Core::LocalFileData("test/Dream/Imaging/9452-v2.jpg");
			// 		Ref<JPEGImage> image = new JPEGImage(data);
			// 		
			// 		PixelBufferLayout2D pixel_layout{image->size()};
			// 		Ref<PixelBuffer2D> pixel_buffer = new PixelBuffer2D(pixel_layout);
			// 		image->convert(pixel_layout, pixel_buffer->data());
			// 		
			// 		examiner << "Correct pixel colour was loaded from file" << std::endl;
			// 		examiner.expect((*pixel_buffer)[{0, 0}]) == Vector<4, Byte>{125, 103, 79, 255};
			// 	}
			// },
			// 
			{"can save to lossless RGBA data",
				[](UnitTest::Examiner & examiner) {
					Ref<IData> data = new Core::LocalFileData("test/Dream/Imaging/test.webp");
					Ref<WebPImage> image = new WebPImage(data);
					
					PixelBufferLayout2D pixel_layout{image->size()};
					Ref<PixelBuffer2D> pixel_buffer = new PixelBuffer2D(pixel_layout);
					image->convert(pixel_layout, pixel_buffer->data());
					
					Ref<IData> output_data = WebPImage::save(pixel_layout, pixel_buffer->data());
					
					examiner << "Output data was generated." << std::endl;
					examiner.expect(output_data->size()) > 0;
					
					Ref<WebPImage> output_image = new WebPImage(output_data);
					examiner << "Output image has same size as input" << std::endl;
					examiner.expect(output_image->size()) == image->size();
				}
			},
			
			{"can save to lossy RGBA data",
				[](UnitTest::Examiner & examiner) {
					Ref<IData> data = new Core::LocalFileData("test/Dream/Imaging/test.webp");
					Ref<WebPImage> image = new WebPImage(data);
					
					PixelBufferLayout2D pixel_layout{image->size()};
					Ref<PixelBuffer2D> pixel_buffer = new PixelBuffer2D(pixel_layout);
					image->convert(pixel_layout, pixel_buffer->data());
					
					Ref<IData> output_data = WebPImage::save(pixel_layout, pixel_buffer->data(), 75);
					
					examiner << "Output data was generated." << std::endl;
					examiner.expect(output_data->size()) > 0;
					
					Ref<WebPImage> output_image = new WebPImage(output_data);
					examiner << "Output image has same size as input" << std::endl;
					examiner.expect(output_image->size()) == image->size();
				}
			},
		};
	}
}
