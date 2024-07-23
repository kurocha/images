//
//  PixelFormat.hpp
//  This file is part of the "Images" project and released under the MIT License.
//
//  Created by Samuel Williams on 22/7/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#include <tuple>
#include <utility>
#include <cstdint>
#include <iostream>
#include <type_traits>

namespace Images
{
	namespace PixelFormat
	{
		using namespace std::rel_ops;
		
		using U8 = std::uint8_t;
		using U16 = std::uint16_t;
		
		template <typename T>
		struct Standard
		{
		};
		
		template <typename T>
		struct Linear
		{
		};
		
		template <typename T>
		struct Associated
		{
		};
		
		template <typename T>
		struct Unassociated
		{
		};
		
		struct RGB8
		{
			U8 red, green, blue;
			
			std::tuple<U8, U8, U8> make_tuple() const noexcept {return std::make_tuple(red, green, blue);}
			
			using PixelTraits = Standard<RGB8>;
		};
		
		struct RGBA8
		{
			U8 red, green, blue, alpha;
			
			std::tuple<U8, U8, U8, U8> make_tuple() const noexcept {return std::make_tuple(red, green, blue, alpha);}
			
			using PixelTraits = Associated<Standard<RGBA8>>;
		};
		
		struct RGBa8
		{
			U8 red, green, blue, alpha;
			
			std::tuple<U8, U8, U8, U8> make_tuple() const noexcept {return std::make_tuple(red, green, blue, alpha);}
			
			using PixelTraits = Unassociated<Standard<RGBA8>>;
		};
		
		struct RGBA16
		{
			U16 red, green, blue, alpha;
			
			std::tuple<U16, U16, U16, U16> make_tuple() const noexcept {return std::make_tuple(red, green, blue, alpha);}
			
			using PixelTraits = Associated<Linear<RGBA16>>;
		};
		
		struct BGR8
		{
			U8 blue, green, red;
			
			std::tuple<U8, U8, U8> make_tuple() const noexcept {return std::make_tuple(blue, green, red);}
			
			using PixelTraits = Standard<BGR8>;
		};
		
		struct BGRA8
		{
			U8 blue, green, red, alpha;
			
			std::tuple<U8, U8, U8, U8> make_tuple() const noexcept {return std::make_tuple(blue, green, red, alpha);}
			
			using PixelTraits = Associated<Standard<BGRA8>>;
		};
		
		struct BGRA16
		{
			U16 blue, green, red, alpha;
			
			std::tuple<U16, U16, U16, U16> make_tuple() const noexcept {return std::make_tuple(blue, green, red, alpha);}
			
			using PixelTraits = Associated<Linear<BGRA16>>;
		};
		
		template <std::size_t N, typename... T>
		typename std::enable_if<(N >= sizeof...(T))>::type
		print_tuple(std::ostream &, const std::tuple<T...> &) {}

		template <std::size_t N, typename... T>
		typename std::enable_if<(N < sizeof...(T))>::type
		print_tuple(std::ostream & output, const std::tuple<T...> & tuple)
		{
			if (N != 0)
				output << ", ";
			
			output << +std::get<N>(tuple);
			
			print_tuple<N+1>(output, tuple);
		}
		
		template <typename T, typename PixelTraits = typename T::PixelTraits>
		std::ostream& operator<<(std::ostream & output, T & t)
		{
			output << '{';
			print_tuple<0>(output, t.make_tuple());
			return output << '}';
		}
		
		template <typename T, typename PixelTraits = typename T::PixelTraits>
		bool operator==(const T & lhs, const T & rhs)
		{
			return lhs.make_tuple() == rhs.make_tuple();
		}
		
		template <typename T, typename PixelTraits = typename T::PixelTraits>
		bool operator<(const T & lhs, const T & rhs)
		{
			return lhs.make_tuple() < rhs.make_tuple();
		}
	}
}
