//
//  PNGImage.cpp
//  This file is part of the "Images" project and released under the MIT License.
//
//  Created by Samuel Williams on 18/4/2016.
//  Copyright, 2016, by Samuel Williams. All rights reserved.
//

#include "PNGImage.hpp"

#include <Resources/LoadError.hpp>

#include <cassert>

extern "C" {
#include <png.h>
}

#include <iostream>

namespace Images
{
	PNGImage::PNGImage(Data * data) : Image(data)
	{
		png_image image;
		image.version = PNG_IMAGE_VERSION;
		image.opaque = nullptr;
		
		png_image_begin_read_from_memory(&image, data->begin(), data->size());
		
		_size[0] = image.width;
		_size[1] = image.height;
		_size[2] = 1;

		png_image_free(&image);
	}
	
	PNGImage::~PNGImage()
	{
	}

	template <typename LayoutT>
	void load_from(Data * input, const LayoutT & layout, int format, Byte * output)
	{
		png_image image = {.opaque = nullptr, .version = PNG_IMAGE_VERSION};
		png_image_begin_read_from_memory(&image, input->begin(), input->size());

		image.format = format;

		png_image_finish_read(&image, nullptr, output, static_cast<png_int_32>(layout.stride[0]), nullptr);

		if (PNG_IMAGE_FAILED(image))
		{
			throw std::runtime_error(image.message);
		}

		png_image_free(&image);
	}

	void PNGImage::load(const PixelLayout<PixelFormat::RGBA8> & layout, Byte * data) const
	{
		PixelLayout<PixelFormat::RGBa8> internal_layout(layout.size, layout.stride);
		
		load_from(_data, internal_layout, PNG_FORMAT_RGBA, data);
		
		auto input_pixels = pixels(internal_layout, data);
		auto output_pixels = pixels(layout, data);
		
		// Perform an in-place conversion:
		internal_layout.convert(layout, input_pixels, output_pixels);
	}

	void PNGImage::load(const PixelLayout<PixelFormat::BGRA8> & layout, Byte * data) const
	{
		load_from(_data, layout, PNG_FORMAT_BGRA, data);
	}

	void PNGImage::load(const PixelLayout<PixelFormat::RGB8> & layout, Byte * data) const
	{
		load_from(_data, layout, PNG_FORMAT_RGB, data);
	}

	void PNGImage::load(const PixelLayout<PixelFormat::BGR8> & layout, Byte * data) const
	{
		load_from(_data, layout, PNG_FORMAT_BGR, data);
	}

	void PNGImage::load(const PixelLayout<PixelFormat::RGBA16> & layout, Byte * data) const
	{
		load_from(_data, layout, PNG_FORMAT_LINEAR_RGB_ALPHA, data);
	}

	void PNGImage::load(const PixelLayout<PixelFormat::BGRA16> & layout, Byte * data) const
	{
		load_from(_data, layout, PNG_FORMAT_LINEAR_RGB_ALPHA|PNG_FORMAT_FLAG_BGR, data);
	}

	template <typename LayoutT>
	Shared<Buffer> save_to_buffer(const LayoutT & layout, png_uint_32 format, const Byte * input)
	{
		png_image image = {.opaque = nullptr, .version = PNG_IMAGE_VERSION, .width = static_cast<png_uint_32>(layout.size[0]), .height = static_cast<png_uint_32>(layout.size[1]), .format = format, .flags = PNG_IMAGE_FLAG_FAST};

		auto buffer = shared<DynamicBuffer>(PNG_IMAGE_PNG_SIZE_MAX(image));

		png_alloc_size_t size = buffer->size();

		png_image_write_to_memory(&image, buffer->begin(), &size, false, input, static_cast<png_int_32>(layout.stride[0]), nullptr);

		buffer->resize(size);

		return buffer;
	}

	Shared<Buffer> PNGImage::save(PixelLayout<PixelFormat::RGBA8> layout, const Byte * data)
	{
		PixelLayout<PixelFormat::RGBa8> internal_layout(layout.size, layout.stride);
		
		auto input_pixels = pixels(layout, data);
		PixelBuffer<PixelLayout<PixelFormat::RGBa8>> output_pixels(internal_layout);
		
		layout.convert(internal_layout, input_pixels, output_pixels);
		
		return save_to_buffer(internal_layout, PNG_FORMAT_RGBA, output_pixels.begin());
	}

	Shared<Buffer> PNGImage::save(PixelLayout<PixelFormat::BGRA8> layout, const Byte * data)
	{
		return save_to_buffer(layout, PNG_FORMAT_BGRA, data);
	}

	Shared<Buffer> PNGImage::save(PixelLayout<PixelFormat::RGB8> layout, const Byte * data)
	{
		return save_to_buffer(layout, PNG_FORMAT_RGB, data);
	}

	Shared<Buffer> PNGImage::save(PixelLayout<PixelFormat::BGR8> layout, const Byte * data)
	{
		return save_to_buffer(layout, PNG_FORMAT_BGR, data);
	}

	Shared<Buffer> PNGImage::save(PixelLayout<PixelFormat::RGBA16> layout, const Byte * data)
	{
		return save_to_buffer(layout, PNG_FORMAT_LINEAR_RGB_ALPHA, data);
	}

	Shared<Buffer> PNGImage::save(PixelLayout<PixelFormat::BGRA16> layout, const Byte * data)
	{
		return save_to_buffer(layout, PNG_FORMAT_LINEAR_RGB_ALPHA|PNG_FORMAT_FLAG_BGR, data);
	}
}
