//
//  PNGImage.cpp
//  This file is part of the "Dream Images" project and released under the MIT License.
//
//  Created by Samuel Williams on 19/4/2016.
//  Copyright, 2016, by Samuel Williams. All rights reserved.
//

#include <UnitTest/UnitTest.hpp>

#include <Images/PNGImage.hpp>
#include <Resources/FileData.hpp>

namespace Images
{
	UnitTest::Suite PNGImageTestSuite {
		"Dream::Images::PNGImage",
		
		{"can load file and size is correct",
			[](UnitTest::Examiner & examiner) {
				auto data = owner<Resources::FileData>("Images/fixtures/Dream.png");
				auto image = owner<PNGImage>(data);
				
				examiner << "Width and size should be loaded correctly" << std::endl;
				examiner.expect(image->size()) == Vec3u{144, 144, 1};
			}
		},
		
		{"can load file and convert to pixel buffer",
			[](UnitTest::Examiner & examiner) {
				auto data = owner<Resources::FileData>("Images/fixtures/Dream.png");
				auto image = owner<PNGImage>(data);
				
				PixelBufferLayout2D pixel_layout{image->size()};
				auto pixel_buffer = owner<PixelBuffer2D>(pixel_layout);
				image->convert(pixel_layout, pixel_buffer->data());
				
				examiner << "Correct pixel colour was loaded from file" << std::endl;
				examiner.expect((*pixel_buffer)[{0, 0}]) == Vector<4, Byte>{255, 255, 255, 0};
			}
		},
		
		{"can load RGB file and convert to RGBA pixel buffer",
			[](UnitTest::Examiner & examiner) {
				auto data = owner<Resources::FileData>("Images/fixtures/Dream.png");
				auto image = owner<PNGImage>(data);
				
				PixelBufferLayout2D pixel_layout{image->size()};
				auto pixel_buffer = owner<PixelBuffer2D>(pixel_layout);
				image->convert(pixel_layout, pixel_buffer->data());
				
				examiner << "Correct pixel colour was loaded from file" << std::endl;
				examiner.expect((*pixel_buffer)[{0, 0}]) == Vector<4, Byte>{255, 0, 0, 255};
				examiner.expect((*pixel_buffer)[{1, 0}]) == Vector<4, Byte>{0, 255, 0, 255};
				examiner.expect((*pixel_buffer)[{2, 0}]) == Vector<4, Byte>{0, 0, 255, 255};
				examiner.expect((*pixel_buffer)[{3, 0}]) == Vector<4, Byte>{255, 255, 255, 255};
				examiner.expect((*pixel_buffer)[{5, 0}]) == Vector<4, Byte>{0, 0, 0, 255};
			}
		},
		
		{"can save to RGBA data",
			[](UnitTest::Examiner & examiner) {
				auto data = owner<Resources::FileData>("Images/fixtures/Dream.png");
				auto image = owner<PNGImage>(data);
				
				PixelBufferLayout2D pixel_layout{image->size()};
				auto pixel_buffer = owner<PixelBuffer2D>(pixel_layout);
				image->convert(pixel_layout, pixel_buffer->data());
				
				auto output_data = PNGImage::save(pixel_layout, pixel_buffer->data());
				
				examiner << "Output data was generated." << std::endl;
				examiner.expect(output_data->size()) > 0;
				
				auto output_image = owner<PNGImage>(output_data);
				examiner << "Output image has same size as input" << std::endl;
				examiner.expect(output_image->size()) == image->size();
			}
		}
	};
}
