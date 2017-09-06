//
//  Test.Transparency.cpp
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
	};
}
