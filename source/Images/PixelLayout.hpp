//
//  PixelLayout.hpp
//  This file is part of the "Images" project and released under the MIT License.
//
//  Created by Samuel Williams on 22/7/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#include "PixelFormat.hpp"

namespace Images
{
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
	Vector<N, std::size_t> calculate_stride(const Vector<N, std::size_t> & size, std::size_t pixel_size) {
		Vector<1, std::size_t> stride(size[0] * pixel_size);
		return calculate_stride(size, stride);
	}
	
	template <std::size_t N, std::size_t M>
	std::size_t byte_offset(std::size_t pixel_size, Vector<N, std::size_t> stride, Vector<M, std::size_t> coordinates)
	{
		static_assert(M <= N, "Coordinate must have less or same number of dimensions.");

		std::size_t offset = 0, current_stride = pixel_size;

		for (std::size_t m = 0; m < M; m += 1) {
			offset += coordinates[m] * current_stride;
			current_stride = stride[m];
		}

		return offset;
	}
	
	/// The layout of pixels in a buffer. Things which affect the actual pixel 
	template <typename PixelFormatT = PixelFormat::RGBA8, std::size_t N = 2>
	struct PixelLayout {
		using Size = Euclid::Numerics::Vector<N, std::size_t>;
		using PixelFormat = PixelFormatT;
		
		Size size, stride;
		
		constexpr static std::size_t pixel_size() {
			return sizeof(PixelFormatT);
		}
		
		std::size_t data_size() const {
			return stride.back();
		}
		
		template <typename PixelFormatT>
		PixelLayout(const PixelLayout<PixelFormatT, N> & other) : size(other.size), stride(other.stride)
		{
			static_assert(pixel_size() == other.pixel_size(), "Pixel elements must have same size");
		}
		
		template <typename StrideType>
		PixelLayout(const Size & _size, const StrideType & _stride) : size(_size), stride(calculate_stride(_size, _stride))
		{
		}
		
		PixelLayout(const Size & _size) : size(_size), stride(calculate_stride(_size, pixel_size()))
		{
		}
		
		template <typename BlockT>
		void each_coordinate(BlockT block, Size & offset, const std::size_t n, const std::size_t m) const {
			for (offset[n] = 0; offset[n] < size[n]; offset[n] += 1) {
				if (n == m)
					block(offset);
				else
					each_coordinate(block, offset, n - 1, m);
			}
		}
		
		template <typename BlockT>
		Size each_coordinate(BlockT block, std::size_t m = 0) const {
			Size offset = 0;
			
			each_coordinate(block, offset, N - 1, m);
			
			return offset;
		}
		
		// Apply a function to every pixel in the given pixel buffer.
		template <typename FunctionT, typename PixelBufferT>
		void apply(PixelBufferT & pixel_buffer, FunctionT function) {
			each_coordinate([&](const Size & coordinate){
				pixel_buffer[coordinate] = function(pixel_buffer[coordinate]);
			});
		}
		
		// Convert pixel format into a new buffer.
		template <typename PixelFormatT, typename InputBufferT, typename OutputBufferT>
		void convert(const PixelLayout<PixelFormatT, N> & output_layout, const InputBufferT & input, OutputBufferT & output)
		{
			each_coordinate([&](const Size & coordinate){
				output[coordinate] = input[coordinate];
			});
		}
		
		template <std::size_t M>
		std::size_t byte_offset(Vector<M, std::size_t> coordinates) {
			return Images::byte_offset(pixel_byte_size(), stride, coordinates);
		}

		template <typename PointerType>
		std::vector<PointerType> generate_row_pointers(PointerType data) {
			std::vector<PointerType> row_pointers;
			
			each_coordinate([&](const Size & offset){
				row_pointers.push_back(data + byte_offset(offset));
			}, 1);
			
			return row_pointers;
		}
	};
	
	using PixelLayout2D = PixelLayout<>;
}
