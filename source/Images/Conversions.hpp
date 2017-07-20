//
//  Alpha.hpp
//  This file is part of the "vulkanraum" project and released under the Orion Transfer Commercial License.
//
//  Created by Samuel Williams on 28/4/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#include <Euclid/Numerics/Vector.hpp>

namespace Images
{
	namespace Conversions
	{
		using namespace Euclid::Numerics;
		
		// Taken from http://entropymine.com/imageworsener/srgbformula/
		template <typename NumericT>
		NumericT convert_srgb_to_linear(NumericT component) {
			if (component <= 0.0404482362771082) {
				return component / 12.92;
			} else {
				return ((component + 0.055) / 1.055).raise(2.4);
			}
		}
		
		template <typename NumericT>
		NumericT convert_linear_to_srgb(NumericT component) {
			if (component <= 0.00313066844250063) {
				return component * 12.92;
			} else {
				return component.raise(1.0/2.4) * 1.055 - 0.055;
			}
		}
		
		// Vector<4, Byte> srgb_convert_to_premultiplied_alpha(Vector<4, Byte> input) {
		// 	auto normalized = Vec4(input) / 255.0;
		// 	
		// 	float alpha = normalized[3];
		// 	
		// 	Vec3 linear;
		// 	for (std::size_t i = 0; i < 3; i += 1) {
		// 		linear[i] = convert_srgb_to_linear(normalized[i]);
		// 	}
		// 	
		// 	auto premultiplied = linear * alpha;
		// 	
		// 	return (convert_linear_to_srgb(premultiplied) * 255.0) << input[3];
		// }
		// 
		// Vector<4, Byte> srgb_convert_to_postmultiplied_alpha(Vector<4, Byte> input) {
		// 	auto normalized = Vec4(input) / 255.0;
		// 	
		// 	float alpha = normalized[3];
		// 	
		// 	Vec3 linear;
		// 	for (std::size_t i = 0; i < 3; i += 1) {
		// 		linear[i] = convert_srgb_to_linear(normalized[i]);
		// 	}
		// 	
		// 	auto premultiplied = linear / alpha;
		// 	
		// 	return (convert_linear_to_srgb(premultiplied) * 255.0) << input[3];
		// }
	}
}
