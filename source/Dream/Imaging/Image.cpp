//
//  Imaging/Image.cpp
//  This file is part of the "Dream" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 1/05/07.
//  Copyright (c) 2007 Samuel Williams. All rights reserved.
//
//

#include "Image.hpp"
#include <cassert>

#include "PNGImage.hpp"
#include "JPEGImage.hpp"

namespace Dream {
	namespace Imaging {
		/* From MacOS X mime.magic:
			# JPEG images
			0	beshort		0xffd8		image/jpeg

			#PNG Image Format
			0	string		\x89PNG			image/png

			# PC bitmaps (OS/2, Windoze BMP files)  (Greg Roelofs, newt@uchicago.edu)
			0	string		BM		image/bmp

			# TIFF and friends
			#					TIFF file, big-endian
			0	string		MM		image/tiff
			#					TIFF file, little-endian
			0	string		II		image/tiff
		*/
		
		enum class ImageType {
			UNKNOWN, JPEG, PNG
		};
		
		static ImageType read_image_type(const Buffer & buffer) {
			if (buffer.size() < 4) return ImageType::UNKNOWN;
			
			if (buffer[0] == 0xFF && buffer[1] == 0xD8)
				return ImageType::JPEG;
			
			if (buffer[0] == 0x89 && buffer[1] == 'P' && buffer[2] == 'N' && buffer[3] == 'G')
				return ImageType::PNG;
			
			return ImageType::UNKNOWN;
		}
		
		Image::Loader::~Loader()
		{
		}
		
		Ref<Object> Image::Loader::load(const Path & path, const ILoader & top) const
		{
			if (auto data = load_next(path, top).as<IData>())
			{
				switch (read_image_type(*data->buffer())) {
					case ImageType::PNG:
						return new PNGImage(data);
					case ImageType::JPEG:
						return new JPEGImage(data);
				}
			}
			
			return nullptr;
		}

		Image::Image(Ptr<IData> data) : _data(data), _size(ZERO)
		{
		}

		Image::~Image ()
		{
		}
	}
}
