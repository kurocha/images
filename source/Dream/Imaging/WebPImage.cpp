//
//  WebPImage.cpp
//  File file is part of the "Dream Imaging" project and released under the MIT License.
//
//  Created by Samuel Williams on 30/1/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include "WebPImage.hpp"

#include <webp/encode.h>
#include <webp/decode.h>

namespace Dream
{
	namespace Imaging
	{
		WebPImage::WebPImage(Ptr<IData> data) : Image(data)
		{
			int width = 0, height = 0;
			
			WebPGetInfo(
				const_cast<Byte *>(data->buffer()->begin()),
				data->buffer()->size(), 
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
				_data->buffer()->begin(),
				_data->buffer()->size(),
				data, layout.data_size(), layout.stride[0]
			);
			
			if (result == nullptr) throw std::runtime_error("WebP decoder failed to convert image!");
		}
		
		Ref<IData> WebPImage::save(PixelBufferLayout2D layout, const Byte * data)
		{
			return nullptr;
		}
	}
}
