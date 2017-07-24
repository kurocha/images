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
#include <iostream>
#include <type_traits>

namespace Images
{
	namespace PixelFormat
	{
		using namespace std::rel_ops;
		
		template <typename ValueT>
		struct Red
		{
			ValueT value;
			
			Red(const ValueT & value_) : value(value_) {}
			
			bool operator==(const Red & other) const noexcept {return value == other.value;}
			bool operator<(const Red & other) const noexcept {return value < other.value;}
		};
		
		template <typename ValueT>
		std::ostream & operator<<(std::ostream & output, const Red<ValueT> & red) {return output << +red.value;}
		
		template <typename ValueT>
		struct Green
		{
			ValueT value;
			
			Green(const ValueT & value_) : value(value_) {}
			
			bool operator==(const Green & other) const noexcept {return value == other.value;}
			bool operator<(const Green & other) const noexcept {return value < other.value;}
		};
		
		template <typename ValueT>
		std::ostream & operator<<(std::ostream & output, const Green<ValueT> & green) {return output << +green.value;}

		template <typename ValueT>
		struct Blue
		{
			ValueT value;
			
			Blue(const ValueT & value_) : value(value_) {}
			
			bool operator==(const Blue & other) const noexcept {return value == other.value;}
			bool operator<(const Blue & other) const noexcept {return value < other.value;}
		};

		template <typename ValueT>
		std::ostream & operator<<(std::ostream & output, const Blue<ValueT> & blue) {return output << +blue.value;}

		template <typename ValueT>
		struct Alpha
		{
			ValueT value;
			
			Alpha(const ValueT & value_) : value(value_) {}
			
			bool operator==(const Alpha & other) const noexcept {return value == other.value;}
			bool operator<(const Alpha & other) const noexcept {return value < other.value;}
		};

		template <typename ValueT>
		std::ostream & operator<<(std::ostream & output, const Alpha<ValueT> & alpha) {return output << +alpha.value;}

		template <typename ...Channels>
		struct Color : public std::tuple<Channels...>
		{
			using std::tuple<Channels...>::tuple;

			static constexpr std::size_t CHANNELS = sizeof...(Channels);
		};

		template <typename ...Channels>
		struct Linear : public Color<Channels...>
		{
			using Color<Channels...>::Color;

			static constexpr std::size_t CHANNELS = sizeof...(Channels);
		};

		template <typename ...Channels>
		struct Standard : public Color<Channels...>
		{
			using Color<Channels...>::Color;

			static constexpr std::size_t CHANNELS = sizeof...(Channels);
		};
		
		// Associated or pre-multiplied alpha (transparency).
		template <typename ...Channels>
		struct Associated : public std::tuple<Channels...>
		{
			using std::tuple<Channels...>::tuple;
			
			static constexpr std::size_t CHANNELS = sizeof...(Channels);
		};
		
		// Unassociated or straight alpha (opacity).
		template <typename ...Channels>
		struct Unassociated : public std::tuple<Channels...>
		{
			using std::tuple<Channels...>::tuple;
			
			static constexpr std::size_t CHANNELS = sizeof...(Channels);
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
			
			output << std::get<N>(tuple);
			
			print_tuple<N+1>(output, tuple);
		}
		
		template <typename... T>
		std::ostream& operator<<(std::ostream & output, const std::tuple<T...> & tuple)
		{
			output << "[";
			print_tuple<0>(output, tuple);
			return output << "]";
		}
		
		using U8 = std::uint8_t;
		using U16 = std::uint16_t;
		
		// All default 8-bit pixel formats assume sRGB color space.
		using BGRA8 = Associated<Standard<Blue<U8>, Green<U8>, Red<U8>>, Alpha<U8>>;
		using RGBA8 = Associated<Standard<Red<U8>, Green<U8>, Blue<U8>>, Alpha<U8>>;

		// Most file formats use unassociated alpha so we use this to convert the data.
		using UBGRA8 = Unassociated<Standard<Blue<U8>, Green<U8>, Red<U8>>, Alpha<U8>>;
		using URGBA8 = Unassociated<Standard<Red<U8>, Green<U8>, Blue<U8>>, Alpha<U8>>;
		
		using BGR8 = Standard<Blue<U8>, Green<U8>, Red<U8>>;
		using RGB8 = Standard<Red<U8>, Green<U8>, Blue<U8>>;
		
		// All default 16-bit pixel formats assume linear color space.
		using BGRA16 = Associated<Linear<Blue<U16>, Green<U16>, Red<U16>>, Alpha<U16>>;
		using RGBA16 = Associated<Linear<Red<U16>, Green<U16>, Blue<U16>>, Alpha<U16>>;
	}
}
