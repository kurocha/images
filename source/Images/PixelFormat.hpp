//
//  PixelFormat.hpp
//  This file is part of the "Images" project and released under the MIT License.
//
//  Created by Samuel Williams on 22/7/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

namespace Images
{
	namespace PixelFormat
	{
		template <typename ValueT>
		struct Red
		{
			ValueT value;
		};

		template <typename ValueT>
		struct Green
		{
			ValueT value;
		};

		template <typename ValueT>
		struct Blue
		{
			ValueT value;
		};

		template <typename ValueT>
		struct Alpha
		{
			ValueT value;
		};

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
			using std::tuple<Channels...>::tuple;

			static constexpr std::size_t CHANNELS = sizeof...(Channels);
		};

		template <typename ...Channels>
		struct Premultiplied : public std::tuple<Channels...>
		{
			using std::tuple<Channels...>::tuple;
			
			static constexpr std::size_t CHANNELS = sizeof...(Channels);
		};
		
		template <typename ...Channels>
		struct Postmultiplied : public std::tuple<Channels...>
		{
			using std::tuple<Channels...>::tuple;
			
			static constexpr std::size_t CHANNELS = sizeof...(Channels);
		};
		
		using U8 = std::uint8_t;
		using U16 = std::uint16_t;
		
		using BGRA8 = Premultiplied<Standard<Blue<U8>, Green<U8>, Red<U8>>, Alpha<U8>>;
		using RGBA8 = Premultiplied<Standard<Red<U8>, Green<U8>, Blue<U8>>, Alpha<U8>>;
		
		using BGR8 = Standard<Blue<U8>, Green<U8>, Red<U8>>;
		using RGB8 = Standard<Red<U8>, Green<U8>, Blue<U8>>;
		
		using BGRA16 = Premultiplied<Linear<Blue<U16>, Green<U16>, Red<U16>>, Alpha<U16>>;
		using RGBA16 = Premultiplied<Linear<Red<U16>, Green<U16>, Blue<U16>>, Alpha<U16>>;
	}
}
