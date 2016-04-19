//
//  Imaging/PixelBuffer.h
//  This file is part of the "Dream" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 30/04/07.
//  Copyright (c) 2007 Samuel Williams. All rights reserved.
//
//

#pragma once

#include <Dream/Framework.hpp>

#include <Dream/Core/Algorithm.hpp>
#include <Dream/Core/Buffer.hpp>

#include <Euclid/Numerics/Vector.hpp>
#include <iostream>

namespace Dream {
	namespace Imaging {
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
		
		template <PixelFormat PIXEL_FORMAT = PixelFormat::RGBA, typename DataType = Byte, std::size_t N = 2>
		class PixelLayout {
		public:
			typedef Euclid::Numerics::Vector<N, std::size_t> SizeType;
			typedef Euclid::Numerics::Vector<channel_count(PIXEL_FORMAT), DataType> PixelType;
			
		protected:
			SizeType _size, _stride;
			
		public:
			constexpr static std::size_t pixel_byte_size() {
				return sizeof(DataType) * channel_count(PIXEL_FORMAT);
			}
			
			const SizeType & size() const { return _size; }
			const SizeType & stride() const { return _stride; }
			
			std::size_t data_size() const {
				return _stride.back();
			}
			
			template <typename StrideType>
			PixelLayout(const SizeType & size, const StrideType & stride) : _size(size), _stride(calculate_stride(size, stride))
			{
			}
			
			PixelLayout(const SizeType & size) : _size(size), _stride(calculate_stride(size, pixel_byte_size()))
			{
			}
			
			template <std::size_t M>
			std::size_t byte_offset(Vector<M, std::size_t> coordinates) {
				return Imaging::byte_offset(pixel_byte_size(), _stride, coordinates);
			}

			template <typename PointerType>
			std::vector<PointerType> generate_row_pointers(PointerType data) {
				std::vector<PointerType> row_pointers;
				SizeType offset = ZERO;
				
				append_row_pointers(data, offset, row_pointers);
				
				return row_pointers;
			}
			
		private:
			template <typename PointerType>
			void append_row_pointers(PointerType data, SizeType & offset, std::vector<PointerType> & pointers, std::size_t n = N - 1)
			{
				if (n == 0) {
					pointers.push_back(data + byte_offset(offset));
				} else {
					for (offset[n] = 0; offset[n] < _size[n]; offset[n] += 1) {
						append_row_pointers(data, offset, pointers, n - 1);
					}
				}
			}
		};

		// A pixel buffer is a container for pixel data along with a layout that allows for interpretation of the buffer contents. This typically includes a pixel format which describes the layout of colour channels within an individual pixel element, a data type which describes the storage for each pixel component and a size which describes the organisation of pixel data into rows and columns and potentially other dimensions.
		class IPixelBuffer : virtual public IObject {
		public:
			virtual ~IPixelBuffer();
			
			virtual const Byte * data () const = 0;
		};

		template <typename PixelLayoutType>
		class PixelBuffer : public Object, virtual public IPixelBuffer {
		protected:
			PixelLayoutType _pixel_layout;
			Core::DynamicBuffer _buffer;

			void resize() {
				_buffer.resize(_pixel_layout.data_size());
			}

		public:
			typedef typename PixelLayoutType::SizeType SizeType;
			typedef typename PixelLayoutType::PixelType PixelType;
			
			PixelBuffer (const PixelLayoutType & pixel_layout) : _pixel_layout(pixel_layout) { resize(); }
			virtual ~PixelBuffer() {}
			
			const Core::MutableBuffer & buffer() const { return _buffer; }
			Core::MutableBuffer & buffer() { return _buffer; }
			
			virtual const Byte * data() const { return _buffer.begin(); }
			virtual Byte * data() { return _buffer.begin(); }
			
			PixelType & operator[] (const SizeType & coordinates) {
				return *reinterpret_cast<PixelType *>(data() + _pixel_layout.byte_offset(coordinates));
			}
			
			const PixelLayoutType & pixel_layout() const { return _pixel_layout; }
		};
		
		// Standard efficient layouts:
		using PixelLayout2D = PixelLayout<>;
		using PixelBuffer2D = PixelBuffer<PixelLayout2D>;
	}
}
