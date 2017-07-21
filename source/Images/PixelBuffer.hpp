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

#include <vector>

namespace Images
{
	using namespace Memory;
	typedef unsigned char Byte;
	
	using namespace Euclid::Numerics;

	/// Image pixel formats.
	enum class PixelFormat : std::uint16_t {
		// Single channel formats:
		R = 0x0101,
		G = 0x0201,
		B = 0x0301,
		A = 0x0401,

		// Full colour formats:
		RGB = 0x0503,
		RGBA = 0x0604,
		BGRA = 0x0704,

		// Luminance formats:
		L = 0x0801,
		LA = 0x0902,
	};

	constexpr std::uint8_t channel_count(PixelFormat pixel_format) {
		return static_cast<std::uint8_t>(pixel_format);
	}
	
	template <std::size_t N>
	Vector<N, std::size_t> calculate_stride(const Vector<N, std::size_t> & size, Vector<N, std::size_t> stride) {
		return stride;
	}
	
	// This function computes the stride of a given multi-dimensional array size with a given initial stride.
	template <std::size_t N, std::size_t M>
	Vector<N, std::size_t> calculate_stride(const Vector<N, std::size_t> & size, Vector<M, std::size_t> stride) {
		return calculate_stride(size, stride << (stride[M-1] * size[M]));
	}
	
	// This function computes the stride of a given multi-dimensional array size with a given initial stride.
	template <std::size_t N>
	Vector<N, std::size_t> calculate_stride(const Vector<N, std::size_t> & size, std::size_t bytes_per_element) {
		Vector<1, std::size_t> stride(size[0] * bytes_per_element);
		return calculate_stride(size, stride);
	}
	
	template <std::size_t N, std::size_t M>
	std::size_t byte_offset(std::size_t bytes_per_element, Vector<N, std::size_t> stride, Vector<M, std::size_t> coordinates)
	{
		static_assert(M <= N, "Coordinate must have less or same number of dimensions.");

		std::size_t offset = 0, current_stride = bytes_per_element;

		for (std::size_t m = 0; m < M; m += 1) {
			offset += coordinates[m] * current_stride;
			current_stride = stride[m];
		}

		return offset;
	}
	
	// Specifies the color space of the data.
	enum class ColorSpace {
		// Data maps linearly from value to color.
		LINEAR = 0,
		// Data maps using sRGB gamma and cromaticity from value to color.
		SRGB = 1
	};
	
	template <PixelFormat PIXEL_FORMAT = PixelFormat::RGBA, typename DataType = Byte, std::size_t N = 2>
	class PixelBufferLayout {
	public:
		typedef Euclid::Numerics::Vector<N, std::size_t> SizeType;
		typedef Euclid::Numerics::Vector<channel_count(PIXEL_FORMAT), DataType> PixelType;
		
		SizeType size, stride;
		ColorSpace color_space = ColorSpace::SRGB;
		
		constexpr static std::size_t pixel_byte_size() {
			return sizeof(DataType) * channel_count(PIXEL_FORMAT);
		}
		
		std::size_t data_size() const {
			return stride.back();
		}
		
		template <typename StrideType>
		PixelBufferLayout(const SizeType & _size, const StrideType & _stride) : size(_size), stride(calculate_stride(_size, _stride))
		{
		}
		
		PixelBufferLayout(const SizeType & _size) : size(_size), stride(calculate_stride(_size, pixel_byte_size()))
		{
		}
		
		template <typename BlockT>
		void each_coordinate(BlockT block, SizeType & offset, const std::size_t n, const std::size_t m) const {
			for (offset[n] = 0; offset[n] < size[n]; offset[n] += 1) {
				if (n == m)
					block(offset);
				else
					each_coordinate(block, offset, n - 1, m);
			}
		}
		
		template <typename BlockT>
		SizeType each_coordinate(BlockT block, std::size_t m = 0) const {
			SizeType offset = 0;
			
			each_coordinate(block, offset, N - 1, m);
			
			return offset;
		}
		
		// Apply a function to every pixel in the given pixel buffer.
		template <typename FunctionT, typename PixelBufferT>
		void apply(PixelBufferT & pixel_buffer, FunctionT function) {
			each_coordinate([&](const SizeType & coordinate){
				pixel_buffer[coordinate] = function(pixel_buffer[coordinate]);
			});
		}
		
		template <std::size_t M>
		std::size_t byte_offset(Vector<M, std::size_t> coordinates) {
			return Images::byte_offset(pixel_byte_size(), stride, coordinates);
		}

		template <typename PointerType>
		std::vector<PointerType> generate_row_pointers(PointerType data) {
			std::vector<PointerType> row_pointers;
			
			each_coordinate([&](const SizeType & offset){
				row_pointers.push_back(data + byte_offset(offset));
			}, 1);
			
			return row_pointers;
		}
	};

	// A pixel buffer is a container for pixel data along with a layout that allows for interpretation of the buffer contents. This typically includes a pixel format which describes the layout of colour channels within an individual pixel element, a data type which describes the storage for each pixel component and a size which describes the organisation of pixel data into rows and columns and potentially other dimensions.
	template <typename PixelBufferLayoutType>
	class PixelBuffer : public Object {
	protected:
		PixelBufferLayoutType _layout;
		Buffers::DynamicBuffer _buffer;

		void resize() {
			_buffer.resize(_layout.data_size());
		}

	public:
		typedef typename PixelBufferLayoutType::SizeType SizeType;
		typedef typename PixelBufferLayoutType::PixelType PixelType;
		
		PixelBuffer (const PixelBufferLayoutType & layout) : _layout(layout) { resize(); }
		virtual ~PixelBuffer() {}
		
		const Buffers::MutableBuffer & buffer() const { return _buffer; }
		Buffers::MutableBuffer & buffer() { return _buffer; }
		
		virtual const Byte * data() const { return _buffer.begin(); }
		virtual Byte * data() { return _buffer.begin(); }
		
		PixelType & operator[] (const SizeType & coordinates) {
			return *reinterpret_cast<PixelType *>(data() + _layout.byte_offset(coordinates));
		}
		
		const PixelBufferLayoutType & layout() const { return _layout; }
	};
	
	// Standard efficient layouts:
	using PixelBufferLayout2D = PixelBufferLayout<>;
	using PixelBuffer2D = PixelBuffer<PixelBufferLayout2D>;
}
