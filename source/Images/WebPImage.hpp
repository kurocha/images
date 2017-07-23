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
		
		// Load the image data into the given pixel buffer with the specified layout.
		void load(const PixelLayout<PixelFormat::RGBA8> & layout, Byte * data) const;
		void load(const PixelLayout<PixelFormat::BGRA8> & layout, Byte * data) const;
		void load(const PixelLayout<PixelFormat::RGB8> & layout, Byte * data) const;
		void load(const PixelLayout<PixelFormat::BGR8> & layout, Byte * data) const;
		
		// Lossless encoding:
		static Shared<Buffer> save(PixelLayout<PixelFormat::RGBA8> layout, const Byte * data);
		static Shared<Buffer> save(PixelLayout<PixelFormat::BGRA8> layout, const Byte * data);
		static Shared<Buffer> save(PixelLayout<PixelFormat::RGB8> layout, const Byte * data);
		static Shared<Buffer> save(PixelLayout<PixelFormat::BGR8> layout, const Byte * data);
		
		// Lossy encoding, quality from 0 to 100.
		static Shared<Buffer> save(PixelLayout<PixelFormat::RGBA8> layout, const Byte * data, std::uint32_t quality);
		static Shared<Buffer> save(PixelLayout<PixelFormat::BGRA8> layout, const Byte * data, std::uint32_t quality);
		static Shared<Buffer> save(PixelLayout<PixelFormat::RGB8> layout, const Byte * data, std::uint32_t quality);
		static Shared<Buffer> save(PixelLayout<PixelFormat::BGR8> layout, const Byte * data, std::uint32_t quality);
	
	protected:
		template <typename FunctionT, typename LayoutT>
		void load(FunctionT function, const LayoutT & layout, Byte * data) const;
		
		template <typename FunctionT, typename LayoutT>
		static Shared<Buffer> save(FunctionT function, const LayoutT & layout, const Byte * data);
		
		template <typename FunctionT, typename LayoutT>
		static Shared<Buffer> save(FunctionT function, const LayoutT & layout, const Byte * data, std::uint32_t quality);
	};
}
