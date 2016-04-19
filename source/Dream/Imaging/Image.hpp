//
//  Imaging/Image.h
//  This file is part of the "Dream" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 30/04/07.
//  Copyright (c) 2007 Samuel Williams. All rights reserved.
//
//

#pragma once

#include "PixelBuffer.hpp"

#include <Dream/Resources/Loader.hpp>

#include <Euclid/Numerics/Vector.IO.hpp>

namespace Dream {
	namespace Imaging {
		using namespace Resources;
		using namespace Euclid::Numerics;
		
		class Image : public Object
		{
		public:
			class Loader : public Object, virtual public ILoadable {
			public:
				virtual void register_loader_types (ILoader * loader);
				virtual Ref<Object> load_from_data (const Ptr<IData> data, const ILoader * loader);
			};
			
			typedef Vector<3, std::size_t> SizeType;
			
		protected:
			Ref<IData> _data;
			SizeType _size;
			
		public:
			Image(Ptr<IData> data);
			virtual ~Image();
			
			const SizeType & size() const { return _size; }
			virtual void convert(PixelLayout2D _layout, Byte * data) = 0;
		};
	}
}
