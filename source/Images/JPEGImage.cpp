//
//  JPEGImage.cpp
//  This file is part of the "Images" project and released under the MIT License.
//
//  Created by Samuel Williams on 18/4/2016.
//  Copyright, 2016, by Samuel Williams. All rights reserved.
//

#include "JPEGImage.hpp"

#include <Buffers/ForeignBuffer.hpp>
#include <Resources/BufferedData.hpp>

namespace Images
{
	JPEGImage::JPEGImage(Data * data) : Image(data)
	{
		_decompressor = tjInitDecompress();
		
		int jpegSubsamp, width, height;
		
		tjDecompressHeader2(
			_decompressor,
			const_cast<Byte *>(data->begin()),
			data->size(),
			&width, &height, &jpegSubsamp
		);
		
		_size = {width, height, 1};
	}
	
	JPEGImage::~JPEGImage()
	{
		tjDestroy(_decompressor);
	}
	
	void JPEGImage::convert(PixelBufferLayout2D layout, Byte * data) const
	{
		if (layout.color_space != ColorSpace::SRGB) throw std::runtime_error("JPEG requires SRGB color space!");
		
		tjDecompress2(
			_decompressor,
			_data->begin(),
			_data->size(),
			data,
			layout.size[WIDTH], layout.stride[0], layout.size[HEIGHT],
			TJPF_RGBX, TJFLAG_FASTDCT
		);
	}
	
	Owned<Data> JPEGImage::save(PixelBufferLayout2D layout, const Byte * data, std::uint32_t quality)
	{
		if (layout.color_space != ColorSpace::SRGB) throw std::runtime_error("JPEG requires SRGB color space!");
		
		std::size_t output_size = 0;
		Byte * output_data = NULL; //!< Memory is allocated by tjCompress2 if _jpegSize == 0

		tjhandle compressor = tjInitCompress();

		tjCompress2(compressor, data, layout.size[WIDTH], layout.stride[0], layout.size[HEIGHT], TJPF_RGBX, &output_data, &output_size, TJSAMP_444, quality, TJFLAG_FASTDCT);
		
		tjDestroy(compressor);
		
		if (output_data) {
			return owned<Resources::BufferedData>(shared<ForeignBuffer<Byte, tjFree>>(output_data, output_size));
		} else {
			return nullptr;
		}
	}
}
