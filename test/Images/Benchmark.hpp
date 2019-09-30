
#pragma once

#include <Time/Statistics.hpp>
#include <iostream>
#include <string_view>

template <typename Output, typename F>
void measure(Output & output, std::string_view name, F f) {
	Time::Statistics statistics;
	
	for (std::size_t i = 0; i <= 10; i += 1) {
		auto sample = statistics.sample();
		
		f();
	}
	
	output << name << " Average Duration: " << statistics.average_duration() << std::endl;
}
