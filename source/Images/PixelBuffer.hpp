//
//  Images/PixelBuffer.h
//  This file is part of the "Images" project and released under the MIT License.
//
//  Created by Samuel Williams on 30/04/07.
//  Copyright (c) 2007 Samuel Williams. All rights reserved.
//
//

#pragma once

#include <Euclid/Numerics/Vector.hpp>

#include <Buffers/DynamicBuffer.hpp>
#include <Memory/Object.hpp>
#include <Memory/Owned.hpp>

#include "PixelLayout.hpp"

namespace Images
{
	using namespace Memory;
	
	// A pixel buffer is a container for pixel data along with a layout that allows for interpretation of the buffer contents. This typically includes a pixel format which describes the layout of colour channels within an individual pixel element, a data type which describes the storage for each pixel component and a size which describes the organisation of pixel data into rows and columns and potentially other dimensions.
	template <typename PixelLayoutT>
	class PixelBuffer : public Object, public Buffers::MutableBuffer
	{
	public:
		using Byte = unsigned char;
		using Size = typename PixelLayoutT::Size;
		using PixelFormat = typename PixelLayoutT::PixelFormat;
		
		PixelBuffer (const PixelLayoutT & layout) : _layout(layout) { resize(); }
		virtual ~PixelBuffer() {}
		
		virtual const Byte * begin() const { return _buffer.begin(); }
		virtual Byte * begin() { return _buffer.begin(); }
		
		virtual std::size_t size() const { return _buffer.size();}
		
		PixelFormat & operator[](const Size & coordinates) {
			return *reinterpret_cast<PixelFormat *>(begin() + _layout.byte_offset(coordinates));
		}
		
		const PixelLayoutT & layout() const { return _layout; }
		
	protected:
		PixelLayoutT _layout;
		Buffers::DynamicBuffer _buffer;
		
		void resize() {
			_buffer.resize(_layout.data_size());
		}
	};
	
	using PixelBuffer2D = PixelBuffer<PixelLayout2D>;
}
