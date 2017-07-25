//
//  Test.WebPImage.cpp
//  This file is part of the "Images" project and released under the MIT License.
//
//  Created by Samuel Williams on 30/1/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include <UnitTest/UnitTest.hpp>

#include <Images/WebPImage.hpp>
#include <Resources/FileData.hpp>

namespace Images
{
	UnitTest::Suite WebPImageTestSuite {
		"Images::WebPImage",
		
		{"can load file and size is correct",
			[](UnitTest::Examiner & examiner) {
				auto data = owned<Resources::FileData>("Images/fixtures/test.webp");
				auto image = owned<WebPImage>(data);
				
				examiner << "Width and size should be loaded correctly" << std::endl;
				examiner.expect(image->size()) == Vec3u{320, 241, 1};
			}
		},
		
		{"can load file and convert to pixel buffer",
			[](UnitTest::Examiner & examiner) {
				auto data = owned<Resources::FileData>("Images/fixtures/test.webp");
				auto image = owned<WebPImage>(data);
				
				PixelLayout2D pixel_layout{image->size()};
				auto pixel_buffer = owned<PixelBuffer2D>(pixel_layout);
				image->load(pixel_layout, pixel_buffer->begin());
				
				examiner << "Correct pixel colour was loaded from file" << std::endl;
				examiner.expect((*pixel_buffer)[{0, 0}]) == PixelFormat::RGBA8{{32, 128, 196}, 255};
			}
		},
		
		{"can save to lossless RGBA data",
			[](UnitTest::Examiner & examiner) {
				auto data = owned<Resources::FileData>("Images/fixtures/test.webp");
				auto image = owned<WebPImage>(data);
				
				PixelLayout2D pixel_layout{image->size()};
				auto pixel_buffer = owned<PixelBuffer2D>(pixel_layout);
				image->load(pixel_layout, pixel_buffer->begin());
				
				auto output_buffer = WebPImage::save(pixel_layout, pixel_buffer->begin());
				auto output_data = owned<BufferedData>(output_buffer);
				
				examiner << "Output data was generated." << std::endl;
				examiner.expect(output_data->size()) > 0;
				
				auto output_image = owned<WebPImage>(output_data);
				examiner << "Output image has same size as input" << std::endl;
				examiner.expect(output_image->size()) == image->size();
				
				//output_data->buffer()->write_to_file("output1.webp");
			}
		},
		
		{"can save to lossy RGBA data",
			[](UnitTest::Examiner & examiner) {
				auto data = owned<Resources::FileData>("Images/fixtures/test.webp");
				auto image = owned<WebPImage>(data);
				
				PixelLayout2D pixel_layout{image->size()};
				auto pixel_buffer = owned<PixelBuffer2D>(pixel_layout);
				image->load(pixel_layout, pixel_buffer->begin());
				
				auto output_buffer = WebPImage::save(pixel_layout, pixel_buffer->begin(), 75);
				auto output_data = owned<BufferedData>(output_buffer);
				
				examiner << "Output data was generated." << std::endl;
				examiner.expect(output_data->size()) > 0;
				
				auto output_image = owned<WebPImage>(output_data);
				examiner << "Output image has same size as input" << std::endl;
				examiner.expect(output_image->size()) == image->size();
				
				//output_data->buffer()->write_to_file("output2.webp");
			}
		},
	};
}
