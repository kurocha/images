
#include <UnitTest/UnitTest.hpp>

#include <Images/PixelBuffer.hpp>
#include <Images/PNGImage.hpp>
#include <Images/WebPImage.hpp>

#include <cstring>

#include <Resources/FileData.hpp>

#include <Time/Statistics.hpp>
#include <iostream>

namespace Images
{
	UnitTest::Suite PixelBufferTestSuite {
		"Images::PixelBuffer",

		{"it can create red/green gradient",
			[](UnitTest::Examiner & examiner) {
				PixelLayout2D pixel_layout(PixelLayout2D::Size{256, 256});
				PixelBuffer2D pixel_buffer(pixel_layout);
				
				examiner << "Image data was allocated" << std::endl;
				examiner.check_equal(pixel_buffer.size(), 256*256*4);
				
				for (std::size_t i = 0; i < 256; i += 1) {
					for (std::size_t j = 0; j < 256; j += 1) {
						pixel_buffer[{i, j}] = PixelFormat::RGBA8{(PixelFormat::U8)i, (PixelFormat::U8)j, 0, 255};
					}
				}
				
				auto output_buffer = PNGImage::save(pixel_layout, pixel_buffer.begin());
				output_buffer->write_to_file("Red-Green Gradient.png");
			}
		},
		
		{"it can create alpha box",
			[](UnitTest::Examiner & examiner) {
				PixelLayout2D pixel_layout(PixelLayout2D::Size{32, 32});
				PixelBuffer2D pixel_buffer(pixel_layout);
				
				for (std::size_t i = 0; i < 32; i += 1) {
					for (std::size_t j = 0; j < 32; j += 1) {
						pixel_buffer[{i, j}] = PixelFormat::RGBA8{128, 128, 128, 128};
					}
				}
				
				PNGImage::save(pixel_layout, pixel_buffer.begin())->write_to_file("Alpha Box.png");
				WebPImage::save(pixel_layout, pixel_buffer.begin())->write_to_file("Alpha Box.webp");
			}
		},
		
		{"it can create alpha gradient",
			[](UnitTest::Examiner & examiner) {
				PixelLayout2D pixel_layout(PixelLayout2D::Size{256, 256});
				PixelBuffer2D pixel_buffer(pixel_layout);
				
				examiner << "Image data was allocated" << std::endl;
				examiner.check_equal(pixel_buffer.size(), 256*256*4);
				
				for (std::size_t i = 0; i < 256; i += 1) {
					for (std::size_t j = 0; j < 256; j += 1) {
						pixel_buffer[{i, j}] = PixelFormat::RGBA8{(PixelFormat::U8)i, (PixelFormat::U8)i, (PixelFormat::U8)i, (PixelFormat::U8)j};
					}
				}
				
				PNGImage::save(pixel_layout, pixel_buffer.begin())->write_to_file("Alpha Gradient.png");
				WebPImage::save(pixel_layout, pixel_buffer.begin())->write_to_file("Alpha Gradient.webp");
			}
		},
		
		{"it can create solid gradient bar",
			[](UnitTest::Examiner & examiner) {
				PixelLayout2D pixel_layout(PixelLayout2D::Size{256, 32});
				PixelBuffer2D pixel_buffer(pixel_layout);
				
				examiner << "Image data was allocated" << std::endl;
				examiner.check_equal(pixel_buffer.size(), 256*32*4);
				
				for (std::size_t i = 0; i < 256; i += 1) {
					for (std::size_t j = 0; j < 32; j += 1) {
						auto c = static_cast<PixelFormat::U8>(i);
						pixel_buffer[{i, j}] = PixelFormat::RGBA8{c, c, c, 255};
					}
				}
				
				PNGImage::save(pixel_layout, pixel_buffer.begin())->write_to_file("Solid Gradient Bar.png");
				WebPImage::save(pixel_layout, pixel_buffer.begin())->write_to_file("Solid Gradient Bar.webp");
			}
		},
		
		{"it can create alpha gradient bar",
			[](UnitTest::Examiner & examiner) {
				PixelLayout2D pixel_layout(PixelLayout2D::Size{256, 32});
				PixelBuffer2D pixel_buffer(pixel_layout);
				
				examiner << "Image data was allocated" << std::endl;
				examiner.check_equal(pixel_buffer.size(), 256*32*4);
				
				for (std::size_t i = 0; i < 256; i += 1) {
					for (std::size_t j = 0; j < 32; j += 1) {
						pixel_buffer[{i, j}] = PixelFormat::RGBA8{0, 0, 0, static_cast<PixelFormat::U8>(255-i)};
					}
				}
				
				PNGImage::save(pixel_layout, pixel_buffer.begin())->write_to_file("Alpha Gradient Bar.png");
				WebPImage::save(pixel_layout, pixel_buffer.begin())->write_to_file("Alpha Gradient Bar.webp");
			}
		},
	};
}
