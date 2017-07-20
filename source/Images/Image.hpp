//
//  Images/Image.h
//  This file is part of the "Images" project and released under the MIT License.
//
//  Created by Samuel Williams on 30/04/07.
//  Copyright (c) 2007 Samuel Williams. All rights reserved.
//
//

#pragma once

#include "PixelBuffer.hpp"

#include <Resources/BufferedData.hpp>
#include <Resources/ChainLoader.hpp>

#include <Euclid/Numerics/Vector.IO.hpp>

namespace Images
{
	using namespace Resources;
	using namespace Memory;
	using namespace Buffers;
	
	using namespace Euclid::Numerics;
	
	class Image : public Object
	{
	public:
		class Loader : public ChainLoader<Image, Data> {
		public:
			using ChainLoader::ChainLoader;
			virtual ~Loader();
			
			virtual Owned<Image> load(const URI::Generic & uri) const;
		};
		
		typedef Vector<3, std::size_t> SizeType;
		
	protected:
		Owned<Data> _data;
		SizeType _size;
		
	public:
		Image(Data * data);
		virtual ~Image();
		
		static Owned<Image> load(Data * data);
		
		const SizeType & size() const { return _size; }
		virtual void convert(PixelBufferLayout2D _layout, Byte * data) const = 0;
	};
}
