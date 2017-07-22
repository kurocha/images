//
//  WebPImage.hpp
//  This file is part of the "Images" project and released under the MIT License.
//
//  Created by Samuel Williams on 30/1/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#include "Image.hpp"

namespace Images
{
	class WebPImage : public Image
	{
	public:
		WebPImage(Data * data);
		virtual ~WebPImage();
		
		virtual void convert(PixelLayout<PixelFormat::RGBA8> layout, Byte * data) const;
		virtual void convert(PixelLayout<PixelFormat::BGRA8> layout, Byte * data) const;
		virtual void convert(PixelLayout<PixelFormat::RGB8> layout, Byte * data) const;
		virtual void convert(PixelLayout<PixelFormat::BGR8> layout, Byte * data) const;
		
		// Lossless encoding:
		static Owned<Data> save(PixelLayout2D layout, const Byte * data);
		
		// Lossy encoding, quality from 0 to 100.
		static Owned<Data> save(PixelLayout2D layout, const Byte * data, std::uint32_t quality_factor);
	};
}
