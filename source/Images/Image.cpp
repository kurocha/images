//
//  Images/Image.cpp
//  This file is part of the "Images" project and released under the MIT License.
//
//  Created by Samuel Williams on 1/05/07.
//  Copyright (c) 2007 Samuel Williams. All rights reserved.
//
//

#include "Image.hpp"
#include <cassert>

#include "PNGImage.hpp"
#include "JPEGImage.hpp"
#include "WebPImage.hpp"

namespace Images {
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
		UNKNOWN, JPEG, PNG, WEBP
	};
	
	static ImageType read_image_type(Data * data) {
		auto size = data->size();
		auto buffer = data->begin();
		
		if (size >= 2 && buffer[0] == 0xFF && buffer[1] == 0xD8)
			return ImageType::JPEG;
		
		if (size >= 4 && buffer[0] == 0x89 && buffer[1] == 'P' && buffer[2] == 'N' && buffer[3] == 'G')
			return ImageType::PNG;
		
		if (size >= 12 && buffer[0] == 'R' && buffer[1] == 'I' && buffer[2] == 'F' && buffer[3] == 'F' && buffer[8] == 'W' && buffer[9] == 'E' && buffer[10] == 'B' && buffer[11] == 'P')
			return ImageType::WEBP;
		
		return ImageType::UNKNOWN;
	}
	
	Image::Loader::~Loader()
	{
	}
	
	Own<Image> Image::Loader::load(const URI::Generic & uri) const
	{
		if (auto data = load_next(uri)) {
			return Image::load(data);
		}
		
		return nullptr;
	}
	
	Image::Image(Data * data) : _data(data), _size(ZERO)
	{
	}
	
	Image::~Image ()
	{
	}
	
	Own<Image> Image::load(Data * data)
	{
		switch (read_image_type(data)) {
			case ImageType::PNG:
				return new PNGImage(data);
			case ImageType::JPEG:
				return new JPEGImage(data);
			case ImageType::WEBP:
				return new WebPImage(data);
			case ImageType::UNKNOWN:
				return nullptr;
		}
		
		return nullptr;
	}
}
