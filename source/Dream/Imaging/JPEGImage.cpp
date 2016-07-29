//
//  JPEGImage.cpp
//  File file is part of the "vulkanraum" project and released under the Orion Transfer Commercial License.
//
//  Created by Samuel Williams on 18/4/2016.
//  Copyright, 2016, by Samuel Williams. All rights reserved.
//

#include "JPEGImage.hpp"

namespace Dream
{
	namespace Imaging
	{
		JPEGImage::JPEGImage(Ptr<IData> data) : Image(data)
		{
			_decompressor = tjInitDecompress();
			
			int jpegSubsamp, width, height;
			
			tjDecompressHeader2(
				_decompressor,
				const_cast<Byte *>(data->buffer()->begin()),
				data->buffer()->size(), 
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
				_data->buffer()->begin(),
				_data->buffer()->size(),
				data,
				layout.size[WIDTH], layout.stride[0], layout.size[HEIGHT],
				TJPF_RGBX, TJFLAG_FASTDCT
			);
		}
		
		Ref<IData> JPEGImage::save(PixelBufferLayout2D layout, const Byte * data, std::uint32_t quality)
		{
			if (layout.color_space != ColorSpace::SRGB) throw std::runtime_error("JPEG requires SRGB color space!");
			
			std::size_t output_size = 0;
			Byte * output_data = NULL; //!< Memory is allocated by tjCompress2 if _jpegSize == 0

			tjhandle compressor = tjInitCompress();

			tjCompress2(compressor, data, layout.size[WIDTH], layout.stride[0], layout.size[HEIGHT], TJPF_RGBX, &output_data, &output_size, TJSAMP_444, quality, TJFLAG_FASTDCT);

			Ref<IData> output = new BufferedData(output_data, output_size);

			tjDestroy(compressor);
			tjFree(output_data);
			
			return output;
		}
	}
}
