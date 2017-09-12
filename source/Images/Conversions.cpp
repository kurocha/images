//
//  Conversions.cpp
//  This file is part of the "Images" project and released under the MIT License.
//
//  Created by Samuel Williams on 28/4/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include "Conversions.hpp"

#include <cmath>

namespace Images
{
	namespace Conversions
	{
		void convert(const float & input, PixelFormat::U8 & output)
		{
			output = static_cast<PixelFormat::U8>(std::round(input));
		}
		
		PixelFormat::U8 round(const float & input)
		{
			PixelFormat::U8 output;
			
			convert(input, output);
			
			return output;
		}
		
		void convert(const PixelFormat::RGBa8 & input, PixelFormat::RGBA8 & output) {
			auto normalized = Vec4(input.red, input.green, input.blue, input.alpha) / 255.0;
			
			auto alpha = normalized[3];
			
			Vec3 linear;
			for (std::size_t i = 0; i < 3; i += 1) {
				linear[i] = convert_srgb_to_linear(normalized[i]);
			}
			
			auto associated = linear * alpha;
			
			output = {
				round(convert_linear_to_srgb(associated[0]) * 255.0),
				round(convert_linear_to_srgb(associated[1]) * 255.0),
				round(convert_linear_to_srgb(associated[2]) * 255.0),
				input.alpha
			};
		}
		
		void convert(const PixelFormat::RGBA8 & input, PixelFormat::RGBa8 & output) {
			auto normalized = Vec4(input.red, input.green, input.blue, input.alpha) / 255.0;
			
			float alpha = normalized[3];
			
			Vec3 linear;
			for (std::size_t i = 0; i < 3; i += 1) {
				linear[i] = convert_srgb_to_linear(normalized[i]);
			}
			
			auto unassociated = linear / alpha;
			
			output = {
				round(convert_linear_to_srgb(unassociated[0]) * 255.0),
				round(convert_linear_to_srgb(unassociated[1]) * 255.0),
				round(convert_linear_to_srgb(unassociated[2]) * 255.0),
				input.alpha
			};
		}
	}
}
