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
	
	template <auto Function, typename LayoutT>
	void WebPImage::convert(const LayoutT & layout, Byte * data) const
	{
		auto result = Function(_data->begin(), _data->size(), data, layout.data_size(), layout.stride[0]);
		
		if (result == nullptr)
			throw std::runtime_error("WebP decoder failed to convert image!");
	}
	
	// Load the image data into the given pixel buffer with the specified layout.
	void WebPImage::convert(const PixelLayout<PixelFormat::RGBA8> & layout, Byte * data) const
	{
		PixelLayout<URGBA8> internal_layout(layout);
		
		this->convert<WebPDecodeRGBAInto>(internal_layout, data);
		
		// Convert from unassociated to associated alpha:
		internal_layout->convert(layout, data);
	}
	
	void WebPImage::convert(const PixelLayout<PixelFormat::BGRA8> & layout, Byte * data) const
	{
		PixelLayout<UBGRA8> internal_layout(layout);
		
		this->convert<WebPDecodeBGRAInto>(internal_layout, data);
		
		// Convert from unassociated to associated alpha:
		internal_layout->convert(layout, data);
	}
	
	void WebPImage::convert(const PixelLayout<PixelFormat::RGB8> & layout, Byte * data) const
	{
		this->convert<WebPDecodeRGBInto>(layout, data);
	}
	
	void WebPImage::convert(const PixelLayout<PixelFormat::BGR8> & layout, Byte * data) const
	{
		this->convert<WebPDecodeBGRInto>(layout, data);
	}
	
	template <auto Function, typename LayoutT>
	Shared<Buffer> WebPImage::save(const LayoutT & layout, const Byte * data)
	{
		Byte * output_data = nullptr;
		
		auto output_size = Function(data, layout.size[0], layout.size[1], layout.stride[0], &output_data);
		
		if (output_data) {
			return shared<ForeignBuffer<void, WebPFree>>(output_data, output_size);
		} else {
			return nullptr;
		}
	}
	
	Shared<Buffer> WebPImage::save(PixelLayout<PixelFormat::RGBA8> layout, const Byte * data)
	{
		PixelBuffer<PixelLayout<URGBA8>> pixel_buffer(layout);
		
		layout->convert(pixel_buffer.layout(), data, pixel_buffer.data());
		
		return this->save<WebPEncodeLosslessRGBA>(pixel_buffer.layout(), pixel_buffer.data());
	}
	
	Shared<Buffer> WebPImage::save(PixelLayout<PixelFormat::BGRA8> layout, const Byte * data)
	{
		PixelBuffer<PixelLayout<UBGRA8>> pixel_buffer(layout);
		
		layout->convert(pixel_buffer.layout(), data, pixel_buffer.data());
		
		return this->save<WebPEncodeLosslessBGRA>(pixel_buffer.layout(), pixel_buffer.data());
	}
	
	Shared<Buffer> WebPImage::save(PixelLayout<PixelFormat::RGB8> layout, const Byte * data)
	{
		return this->save<WebPEncodeLosslessRGB>(layout, data);
	}
	
	Shared<Buffer> WebPImage::save(PixelLayout<PixelFormat::BGR8> layout, const Byte * data)
	{
		return this->save<WebPEncodeLosslessBGR>(layout, data);
	}
	
	template <auto Function, typename LayoutT>
	Shared<Buffer> WebPImage::save(const LayoutT & layout, const Byte * data, std::uint32_t quality_factor)
	{
		Byte * output_data = nullptr;
		
		auto output_size = Function(data, layout.size[0], layout.size[1], layout.stride[0], quality_factor, &output_data);
		
		if (output_data) {
			return shared<ForeignBuffer<void, WebPFree>>(output_data, output_size);
		} else {
			return nullptr;
		}
	}
	
	Shared<Buffer> WebPImage::save(PixelLayout<PixelFormat::RGBA8> layout, const Byte * data, std::uint32_t quality_factor)
	{
		PixelBuffer<PixelLayout<URGBA8>> pixel_buffer(layout);
		
		layout->convert(pixel_buffer.layout(), data, pixel_buffer.data());
		
		return this->save<WebPEncodeRGBA>(pixel_buffer.layout(), pixel_buffer.data(), quality_factor);
	}
	
	Shared<Buffer> WebPImage::save(PixelLayout<PixelFormat::BGRA8> layout, const Byte * data, std::uint32_t quality_factor)
	{
		PixelBuffer<PixelLayout<UBGRA8>> pixel_buffer(layout);
		
		layout->convert(pixel_buffer.layout(), data, pixel_buffer.data());
		
		return this->save<WebPEncodeBGRA>(pixel_buffer.layout(), pixel_buffer.data(), quality_factor);
	}
	
	Shared<Buffer> WebPImage::save(PixelLayout<PixelFormat::RGB8> layout, const Byte * data, std::uint32_t quality_factor)
	{
		return this->save<WebPEncodeRGB>(layout, data, quality_factor);
	}
	
	Shared<Buffer> WebPImage::save(PixelLayout<PixelFormat::BGR8> layout, const Byte * data, std::uint32_t quality_factor)
	{
		return this->save<WebPEncodeBGR>(layout, data, quality_factor);
	}
}
