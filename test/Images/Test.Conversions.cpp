//
//  PNGImage.cpp
//  This file is part of the "Images" project and released under the MIT License.
//
//  Created by Samuel Williams on 19/4/2016.
//  Copyright, 2016, by Samuel Williams. All rights reserved.
//

#include <UnitTest/UnitTest.hpp>

#include <Images/Conversions.hpp>

namespace Images
{
	using namespace Numerics;
	
	UnitTest::Suite ConversionsTestSuite {
		"Images::Conversions",
		
		{"can convert from SRGB to Linear",
			[](UnitTest::Examiner & examiner) {
				examiner.expect(Conversions::convert_srgb_to_linear(number(0.0))) == 0.0;
				examiner.expect(Conversions::convert_srgb_to_linear(number(1.0))) == 1.0;
				
				for (auto f = number(0.0); f <= 1.0; f += 0.1) {
					auto g = Conversions::convert_srgb_to_linear(number(f));
					auto h = Conversions::convert_linear_to_srgb(number(g));
					
					examiner << "Round trip conversion of sRGB curve." << std::endl;
					examiner.check(f.equivalent(h));
				}
			}
		},
	};
}
