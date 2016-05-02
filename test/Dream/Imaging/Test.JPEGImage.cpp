//
//  JPEGImage.cpp
//  File file is part of the "Dream Imaging" project and released under the MIT License.
//
//  Created by Samuel Williams on 2/5/2016.
//  Copyright, 2016, by Samuel Williams. All rights reserved.
//

#include <UnitTest/UnitTest.hpp>

#include <Dream/Imaging/JPEGImage.hpp>

namespace Dream
{
	namespace Imaging
	{
		UnitTest::Suite JPEGImageTestSuite {
			"Dream::Imaging::JPEGImage",
			
			{"can load file and size is correct",
				[](UnitTest::Examiner & examiner) {
					Ref<IData> data = new Core::LocalFileData("test/Dream/Imaging/9452-v2.jpg");
					Ref<JPEGImage> image = new JPEGImage(data);
					
					examiner << "Width and size should be loaded correctly" << std::endl;
					examiner.expect(image->size()) == Vec3u{64, 64, 1};
				}
			},
			
			{"can load file and convert to pixel buffer",
				[](UnitTest::Examiner & examiner) {
					Ref<IData> data = new Core::LocalFileData("test/Dream/Imaging/9452-v2.jpg");
					Ref<JPEGImage> image = new JPEGImage(data);
					
					PixelBufferLayout2D pixel_layout{image->size()};
					Ref<PixelBuffer2D> pixel_buffer = new PixelBuffer2D(pixel_layout);
					image->convert(pixel_layout, pixel_buffer->data());
					
					examiner << "Correct pixel colour was loaded from file" << std::endl;
					examiner.expect((*pixel_buffer)[{0, 0}]) == Vector<4, Byte>{125, 103, 79, 255};
				}
			},
			
			{"can save to RGB data",
				[](UnitTest::Examiner & examiner) {
					Ref<IData> data = new Core::LocalFileData("test/Dream/Imaging/9452-v2.jpg");
					Ref<JPEGImage> image = new JPEGImage(data);
					
					PixelBufferLayout2D pixel_layout{image->size()};
					Ref<PixelBuffer2D> pixel_buffer = new PixelBuffer2D(pixel_layout);
					image->convert(pixel_layout, pixel_buffer->data());
					
					Ref<IData> output_data = JPEGImage::save(pixel_layout, pixel_buffer->data());
					
					examiner << "Output data was generated." << std::endl;
					examiner.expect(output_data->size()) > 0;
					
					Ref<JPEGImage> output_image = new JPEGImage(output_data);
					examiner << "Output image has same size as input" << std::endl;
					examiner.expect(output_image->size()) == image->size();
				}
			},
		};
	}
}
