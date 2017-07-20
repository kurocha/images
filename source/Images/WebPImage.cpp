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
	
	void WebPImage::convert(PixelBufferLayout2D layout, Byte * data) const
	{
		if (layout.color_space != ColorSpace::SRGB) throw std::runtime_error("WebP requires SRGB color space!");
		
		auto result = WebPDecodeRGBAInto(
			_data->begin(),
			_data->size(),
			data, layout.data_size(), layout.stride[0]
		);
		
		if (result == nullptr) throw std::runtime_error("WebP decoder failed to convert image!");
	}
	
	Own<Data> WebPImage::save(PixelBufferLayout2D layout, const Byte * data)
	{
		Byte * output_data = nullptr;
		
		auto output_size = WebPEncodeLosslessRGBA(data, layout.size[0], layout.size[1], layout.stride[0], &output_data);
		
		if (output_data) {
			return owner<BufferedData>(shared<ForeignBuffer<void, WebPFree>>(output_data, output_size));
		} else {
			return nullptr;
		}
	}
	
	Own<Data> WebPImage::save(PixelBufferLayout2D layout, const Byte * data, std::uint32_t quality_factor)
	{
		Byte * output_data = nullptr;
		
		auto output_size = WebPEncodeRGBA(data, layout.size[0], layout.size[1], layout.stride[0], quality_factor, &output_data);
		
		if (output_data) {
			return owner<BufferedData>(shared<ForeignBuffer<void, WebPFree>>(output_data, output_size));
		} else {
			return nullptr;
		}
	}
}
