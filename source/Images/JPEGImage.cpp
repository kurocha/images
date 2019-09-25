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

extern "C" {
#include <turbojpeg.h>
}

namespace Images
{
	JPEGImage::JPEGImage(Data * data) : Image(data)
	{
		_decompressor = tjInitDecompress();
		
		int jpegSubsamp, width, height, jpegColorspace;
		
		tjDecompressHeader3(
			(tjhandle)_decompressor,
			data->begin(),
			data->size(),
			&width, &height,
			&jpegSubsamp,
			&jpegColorspace
		);
		
		_size = {width, height, 1};
	}
	
	JPEGImage::~JPEGImage()
	{
		tjDestroy((tjhandle)_decompressor);
	}
	
	void JPEGImage::load(const PixelLayout<PixelFormat::RGBA8> & layout, Byte * data) const
	{
		tjDecompress2(
			(tjhandle)_decompressor,
			_data->begin(),
			_data->size(),
			data,
			static_cast<int>(layout.size[WIDTH]), static_cast<int>(layout.stride[0]), static_cast<int>(layout.size[HEIGHT]),
			TJPF_RGBA, TJFLAG_FASTDCT
		);
	}
	
	void JPEGImage::load(const PixelLayout<PixelFormat::BGRA8> & layout, Byte * data) const
	{
		tjDecompress2(
			(tjhandle)_decompressor,
			_data->begin(),
			_data->size(),
			data,
			static_cast<int>(layout.size[WIDTH]), static_cast<int>(layout.stride[0]), static_cast<int>(layout.size[HEIGHT]),
			TJPF_BGRA, TJFLAG_FASTDCT
		);
	}
	
	void JPEGImage::load(const PixelLayout<PixelFormat::RGB8> & layout, Byte * data) const
	{
		tjDecompress2(
			(tjhandle)_decompressor,
			_data->begin(),
			_data->size(),
			data,
			static_cast<int>(layout.size[WIDTH]), static_cast<int>(layout.stride[0]), static_cast<int>(layout.size[HEIGHT]),
			TJPF_RGB, TJFLAG_FASTDCT
		);
	}
	
	void JPEGImage::load(const PixelLayout<PixelFormat::BGR8> & layout, Byte * data) const
	{
		tjDecompress2(
			(tjhandle)_decompressor,
			_data->begin(),
			_data->size(),
			data,
			static_cast<int>(layout.size[WIDTH]), static_cast<int>(layout.stride[0]), static_cast<int>(layout.size[HEIGHT]),
			TJPF_BGR, TJFLAG_FASTDCT
		);
	}

	template <typename LayoutT>
	static Shared<Buffer> save_to_buffer(const LayoutT & layout, const Byte * input, int format, std::uint32_t quality)
	{
		std::size_t output_size = 0;
		Byte * output_data = NULL; //!< Memory is allocated by tjCompress2 if _jpegSize == 0

		tjhandle compressor = tjInitCompress();

		tjCompress2(compressor, input, static_cast<int>(layout.size[WIDTH]), static_cast<int>(layout.stride[0]), static_cast<int>(layout.size[HEIGHT]), TJPF_RGBX, &output_data, &output_size, TJSAMP_444, quality, TJFLAG_FASTDCT);

		tjDestroy(compressor);

		if (output_data) {
			return shared<ForeignBuffer<Byte, tjFree>>(output_data, output_size);
		} else {
			return nullptr;
		}
	}
	
	Shared<Buffer> JPEGImage::save(PixelLayout<PixelFormat::RGBA8> layout, const Byte * input, std::uint32_t quality)
	{
		PixelLayout<PixelFormat::RGBa8> internal_layout(layout.size, layout.stride);
		
		auto input_pixels = pixels(layout, input);
		PixelBuffer<PixelLayout<PixelFormat::RGBa8>> output_pixels(internal_layout);
		
		layout.convert(internal_layout, input_pixels, output_pixels);
		
		return save_to_buffer(internal_layout, output_pixels.begin(), TJPF_RGBA, quality);
	}

	Shared<Buffer> JPEGImage::save(PixelLayout<PixelFormat::BGRA8> layout, const Byte * input, std::uint32_t quality)
	{
		return save_to_buffer(layout, input, TJPF_BGRA, quality);
	}

	Shared<Buffer> JPEGImage::save(PixelLayout<PixelFormat::RGB8> layout, const Byte * input, std::uint32_t quality)
	{
		return save_to_buffer(layout, input, TJPF_RGB, quality);
	}

	Shared<Buffer> JPEGImage::save(PixelLayout<PixelFormat::BGR8> layout, const Byte * input, std::uint32_t quality)
	{
		return save_to_buffer(layout, input, TJPF_BGR, quality);
	}
}
