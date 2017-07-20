//
//  JPEGImage.hpp
//  This file is part of the "Images" project and released under the MIT License.
//
//  Created by Samuel Williams on 18/4/2016.
//  Copyright, 2016, by Samuel Williams. All rights reserved.
//

#pragma once

#include "Image.hpp"

extern "C" {
#include <turbojpeg.h>
}

namespace Images
{
	class JPEGImage : public Image
	{
	public:
		JPEGImage(Data * data);
		virtual ~JPEGImage();
		
		virtual void convert(PixelBufferLayout2D layout, Byte * data) const;
		
		static Owned<Data> save(PixelBufferLayout2D layout, const Byte * data, std::uint32_t quality = 75);
		
	private:
		tjhandle _decompressor;
	};
}
