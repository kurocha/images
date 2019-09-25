//
//  JPEGImage.hpp
//  This file is part of the "Images" project and released under the MIT License.
//
//  Created by Samuel Williams on 18/4/2016.
//  Copyright, 2016, by Samuel Williams. All rights reserved.
//

#pragma once

#include "Image.hpp"

namespace Images
{
	class JPEGImage : public Image
	{
	public:
		JPEGImage(Data * data);
		virtual ~JPEGImage();
		
		void load(const PixelLayout<PixelFormat::RGBA8> & layout, Byte * data) const;
		void load(const PixelLayout<PixelFormat::BGRA8> & layout, Byte * data) const;
		void load(const PixelLayout<PixelFormat::RGB8> & layout, Byte * data) const;
		void load(const PixelLayout<PixelFormat::BGR8> & layout, Byte * data) const;
		
		static Shared<Buffer> save(PixelLayout<PixelFormat::RGBA8> layout, const Byte * data, std::uint32_t quality = 75);
		static Shared<Buffer> save(PixelLayout<PixelFormat::BGRA8> layout, const Byte * data, std::uint32_t quality = 75);
		static Shared<Buffer> save(PixelLayout<PixelFormat::RGB8> layout, const Byte * data, std::uint32_t quality = 75);
		static Shared<Buffer> save(PixelLayout<PixelFormat::BGR8> layout, const Byte * data, std::uint32_t quality = 75);
		
	private:
		void * _decompressor;
	};
}
