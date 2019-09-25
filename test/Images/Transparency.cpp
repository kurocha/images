//
//  Transparency.cpp
//  This file is part of the "Images" project and released under the MIT License.
//
//  Created by Samuel Williams on 6/9/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include <UnitTest/UnitTest.hpp>

#include <Images/PNGImage.hpp>
#include <Images/WebPImage.hpp>
#include <Resources/FileData.hpp>

namespace Images
{
	UnitTest::Suite TransparencyTestSuite {
		"Images::Transparency",
		
		{"it should be associated alpha",
			[](UnitTest::Examiner & examiner) {
				auto data = owned<Resources::FileData>("Images/fixtures/bacon.png");
				auto image = owned<PNGImage>(data);
				
				PixelLayout2D pixel_layout{image->size()};
				auto pixel_buffer = owned<PixelBuffer2D>(pixel_layout);
				image->load(pixel_layout, pixel_buffer->begin());
				
				std::size_t count = 0;
				pixel_layout.each_coordinate([&](const PixelLayout2D::Size & coordinate){
					auto pixel = (*pixel_buffer)[coordinate];
					
					if (pixel.alpha == 0) {
						if (pixel.red != 0) count++;
						if (pixel.green != 0) count++;
						if (pixel.blue != 0) count++;
					}
				});
				
				examiner << "Alpha is 0, color should be 0 too." << std::endl;
				examiner.expect(count) == 0;
			}
		},
		
		{"it should round trip pixels correctly",
			[](UnitTest::Examiner & examiner) {
				PixelLayout2D pixel_layout(PixelLayout2D::Size{256, 256});
				PixelBuffer2D pixel_buffer(pixel_layout);
				
				for (std::size_t i = 0; i < 256; i += 1) {
					for (std::size_t j = 0; j < 256; j += 1) {
						auto a = static_cast<PixelFormat::U8>(i), c = static_cast<PixelFormat::U8>(std::min(i,j));
						
						pixel_buffer[{i,j}] = PixelFormat::RGBA8{c, c, c, a};
					}
				}
				
				// Save the PNG data:
				auto output_buffer = PNGImage::save(pixel_layout, pixel_buffer.begin());
				auto output_data = owned<BufferedData>(output_buffer);
				
				// Load the PNG data:
				PNGImage image{output_data};
				image.load(pixel_layout, pixel_buffer.begin());
				
				for (std::size_t i = 0; i < 256; i += 1) {
					for (std::size_t j = 0; j < 256; j += 1) {
						auto a = static_cast<PixelFormat::U8>(i), c = static_cast<PixelFormat::U8>(std::min(i,j));
						
						examiner.expect(pixel_buffer[{i,j}]) == PixelFormat::RGBA8{c, c, c, a};
					}
				}
			}
		}
	};
}
