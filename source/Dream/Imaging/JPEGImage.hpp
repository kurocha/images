//
//  JPEGImage.hpp
//  File file is part of the "vulkanraum" project and released under the Orion Transfer Commercial License.
//
//  Created by Samuel Williams on 18/4/2016.
//  Copyright, 2016, by Samuel Williams. All rights reserved.
//

#pragma once

#include "Image.hpp"

extern "C" {
#include <turbojpeg.h>
}

namespace Dream
{
	namespace Imaging
	{
		class JPEGImage : public Image
		{
		public:
			JPEGImage(Ptr<IData> data);
			virtual ~JPEGImage();
			
			virtual void convert(PixelBufferLayout2D layout, Byte * data) const;
			
			static Ref<IData> save(PixelBufferLayout2D layout, const Byte * data, std::uint32_t quality = 75);
			
		private:
			tjhandle _decompressor;
		};
	}
}
