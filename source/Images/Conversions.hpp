//
//  Conversions.hpp
//  This file is part of the "Images" project and released under the MIT License.
//
//  Created by Samuel Williams on 28/4/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#include <Numerics/Vector.hpp>

#include "PixelFormat.hpp"

namespace Images
{
	namespace Conversions
	{
		using namespace Numerics;
		
		// Taken from http://entropymine.com/imageworsener/srgbformula/
		template <typename NumericT>
		NumericT convert_srgb_to_linear(NumericT component) {
			if (component <= 0.0404482362771082) {
				return component / 12.92;
			} else {
				return std::pow((component + static_cast<NumericT>(0.055)) / static_cast<NumericT>(1.055), static_cast<NumericT>(2.4));
			}
		}
		
		template <typename NumericT>
		NumericT convert_linear_to_srgb(NumericT component) {
			if (component <= 0.00313066844250063) {
				return component * 12.92;
			} else {
				return std::pow(component, static_cast<NumericT>(1.0/2.4)) * 1.055 - 0.055;
			}
		}
		
		void convert(const PixelFormat::RGBa8 & input, PixelFormat::RGBA8 & output);
		void convert(const PixelFormat::RGBA8 & input, PixelFormat::RGBa8 & output);
	}
}
