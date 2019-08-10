#pragma once

#include <cstdint>
#include <chrono>

namespace timing {
	using std::chrono::microseconds;
	using std::chrono::high_resolution_clock;
	typedef std::chrono::time_point<high_resolution_clock, microseconds> microsecondTimePoint;
}