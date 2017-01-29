//
//  WebPImage.hpp
//  File file is part of the "Dream Imaging" project and released under the MIT License.
//
//  Created by Samuel Williams on 30/1/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#include "Image.hpp"

namespace Dream
{
	namespace Imaging
	{
		class WebPImage : public Image
		{
		public:
			WebPImage(Ptr<IData> data);
			virtual ~WebPImage();
			
			virtual void convert(PixelBufferLayout2D layout, Byte * data) const;
			
			static Ref<IData> save(PixelBufferLayout2D layout, const Byte * data);
		};
	}
}
