//
//  WebPImage.cpp
//  This file is part of the "Images" project and released under the MIT License.
//
//  Created by Samuel Williams on 30/1/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include "WebPImage.hpp"

#include <Buffers/ForeignBuffer.hpp>

#include <webp/encode.h>
#include <webp/decode.h>

namespace Images
{
	WebPImage::WebPImage(Data * data) : Image(data)
	{
		int width = 0, height = 0;
		
		WebPGetInfo(
			const_cast<Byte *>(data->begin()),
			data->size(),
			&width, &height
		);
		
		_size = {width, height, 1};
	}
	
	WebPImage::~WebPImage()
	{
	}
	
	template <typename FunctionT, typename LayoutT>
	void WebPImage::load(FunctionT function, const LayoutT & layout, Byte * data) const
	{
		auto result = function(_data->begin(), _data->size(), data, layout.data_size(), static_cast<int>(layout.stride[0]));
		
		if (result == nullptr)
			throw std::runtime_error("WebP decoder failed to load image!");
	}
	
	// Load the image data into the given pixel buffer with the specified layout.
	void WebPImage::load(const PixelLayout<PixelFormat::RGBA8> & layout, Byte * data) const
	{
		PixelLayout<PixelFormat::RGBa8> internal_layout(layout.size, layout.stride);
		PixelBuffer<PixelLayout<PixelFormat::RGBa8>> input_pixels(internal_layout);
		
		this->load(WebPDecodeRGBAInto, internal_layout, input_pixels.begin());
		
		auto output_pixels = pixels(layout, data);
		
		internal_layout.convert(layout, input_pixels, output_pixels);
	}
	
	void WebPImage::load(const PixelLayout<PixelFormat::BGRA8> & layout, Byte * data) const
	{
		this->load(WebPDecodeBGRAInto, layout, data);
	}
	
	void WebPImage::load(const PixelLayout<PixelFormat::RGB8> & layout, Byte * data) const
	{
		this->load(WebPDecodeRGBInto, layout, data);
	}
	
	void WebPImage::load(const PixelLayout<PixelFormat::BGR8> & layout, Byte * data) const
	{
		this->load(WebPDecodeBGRInto, layout, data);
	}
	
	template <typename FunctionT, typename LayoutT>
	Shared<Buffer> WebPImage::save(FunctionT function, const LayoutT & layout, const Byte * data)
	{
		Byte * output_data = nullptr;
		
		auto output_size = function(data, static_cast<int>(layout.size[0]), static_cast<int>(layout.size[1]), static_cast<int>(layout.stride[0]), &output_data);
		
		if (output_data) {
			return shared<ForeignBuffer<void, WebPFree>>(output_data, output_size);
		} else {
			return nullptr;
		}
	}
	
	Shared<Buffer> WebPImage::save(PixelLayout<PixelFormat::RGBA8> layout, const Byte * data)
	{
		PixelLayout<PixelFormat::RGBa8> internal_layout(layout.size, layout.stride);
		
		auto input_pixels = pixels(layout, data);
		PixelBuffer<PixelLayout<PixelFormat::RGBa8>> output_pixels(internal_layout);
		
		layout.convert(internal_layout, input_pixels, output_pixels);
		
		return save(WebPEncodeLosslessRGBA, internal_layout, output_pixels.begin());
	}
	
	Shared<Buffer> WebPImage::save(PixelLayout<PixelFormat::BGRA8> layout, const Byte * data)
	{
		return save(WebPEncodeLosslessBGRA, layout, data);
	}
	
	Shared<Buffer> WebPImage::save(PixelLayout<PixelFormat::RGB8> layout, const Byte * data)
	{
		return save(WebPEncodeLosslessRGB, layout, data);
	}
	
	Shared<Buffer> WebPImage::save(PixelLayout<PixelFormat::BGR8> layout, const Byte * data)
	{
		return save(WebPEncodeLosslessBGR, layout, data);
	}
	
	template <typename FunctionT, typename LayoutT>
	Shared<Buffer> WebPImage::save(FunctionT function, const LayoutT & layout, const Byte * data, std::uint32_t quality)
	{
		Byte * output_data = nullptr;
		
		auto output_size = function(data, static_cast<int>(layout.size[0]), static_cast<int>(layout.size[1]), static_cast<int>(layout.stride[0]), quality, &output_data);
		
		if (output_data) {
			return shared<ForeignBuffer<void, WebPFree>>(output_data, output_size);
		} else {
			return nullptr;
		}
	}
	
	Shared<Buffer> WebPImage::save(PixelLayout<PixelFormat::RGBA8> layout, const Byte * data, std::uint32_t quality)
	{
		PixelLayout<PixelFormat::RGBa8> internal_layout(layout.size, layout.stride);
		
		auto input_pixels = pixels(layout, data);
		PixelBuffer<PixelLayout<PixelFormat::RGBa8>> output_pixels(internal_layout);
		
		layout.convert(internal_layout, input_pixels, output_pixels);
		
		return save(WebPEncodeRGBA, internal_layout, output_pixels.begin(), quality);
	}
	
	Shared<Buffer> WebPImage::save(PixelLayout<PixelFormat::BGRA8> layout, const Byte * data, std::uint32_t quality)
	{
		return save(WebPEncodeBGRA, layout, data, quality);
	}
	
	Shared<Buffer> WebPImage::save(PixelLayout<PixelFormat::RGB8> layout, const Byte * data, std::uint32_t quality)
	{
		return save(WebPEncodeRGB, layout, data, quality);
	}
	
	Shared<Buffer> WebPImage::save(PixelLayout<PixelFormat::BGR8> layout, const Byte * data, std::uint32_t quality)
	{
		return save(WebPEncodeBGR, layout, data, quality);
	}
}
