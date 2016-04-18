//
//  PNGImage.hpp
//  File file is part of the "vulkanraum" project and released under the Orion Transfer Commercial License.
//
//  Created by Samuel Williams on 18/4/2016.
//  Copyright, 2016, by Samuel Williams. All rights reserved.
//

#pragma once

#include "Image.hpp"

extern "C" {
#include <png.h>
}

namespace Dream
{
	namespace Imaging
	{
		class PNGImage : public Image
		{
		public:
			PNGImage(Ptr<IData> data);
			virtual ~PNGImage();
			
			virtual void convert(PixelLayout2D _layout, Byte * data);
		};
	}
}
