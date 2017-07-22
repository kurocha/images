
#include <UnitTest/UnitTest.hpp>

#include <Images/PixelBuffer.hpp>
#include <Euclid/Numerics/Vector.IO.hpp>

#include <cstring>

namespace Images
{
	UnitTest::Suite PixelBufferTestSuite {
		"Images::PixelBuffer",

		{"PixelBuffer Initialization",
			[](UnitTest::Examiner & examiner) {
				PixelLayout2D pixel_layout(PixelLayout2D::Size{8, 8});
				auto pixel_buffer = owned<PixelBuffer2D>(pixel_layout);

				examiner << "Image data was allocated" << std::endl;
				examiner.check_equal(pixel_buffer->buffer().size(), 8*8*4);

/*
				image->fill(0xFF);

				examiner << "Image was filled with white" << std::endl;
				auto pixels = reader(*image);

				Vector<3, Byte> pixel = pixels[Vec2(0, 0)], white(255, 255, 255);

				examiner.check_equal(pixel, white);

				Owned<Image> small_black_image = new Image({4, 4}, PixelFormat::L, DataType::BYTE);
				small_black_image->fill(0);

				examiner << "Copy black image into corner of white image" << std::endl;
				writer(*image).copy(reader(*small_black_image), 0, 0, small_black_image->size());

				const unsigned char black_row[] = {0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF};
				const unsigned char white_row[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

				for (std::size_t i = 0; i < 4; i += 1)
					examiner.check_equal(std::memcmp(pixels[Vec2(0, i)], black_row, 8), 0);

				for (std::size_t i = 4; i < 8; i += 1)
					examiner.check_equal(std::memcmp(pixels[Vec2(0, i)], white_row, 8), 0);
*/
			}
		},
/*
		{"Pixel Writer",
			[](UnitTest::Examiner & examiner) {
				Owned<Image> image = new Image({8, 8}, PixelFormat::RGB, DataType::BYTE);

				examiner << "Image data was allocated" << std::endl;
				examiner.check_equal(image->buffer().size(), 8*8*3);

				image->fill(0xFF);

				Vector<3, Byte> color {0x11, 0x22, 0x33};
				writer(*image).set({0, 0}, color);

				Vector<3, Byte> output;
				reader(*image).get<3>({0, 0}, output);

				examiner << "Check the colour is correct" << std::endl;
				examiner.check_equal(output, color);
			}
		}
		*/
	};
}
