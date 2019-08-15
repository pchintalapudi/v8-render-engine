#include <random>
#include <chrono>
#include "timing/high_resolution_time.h"

using namespace std::chrono;

static std::mt19937 rng(time_point_cast<milliseconds>(system_clock::now()).time_since_epoch().count());
static std::uniform_real_distribution<double> clockRandomizer{-2.5, 2.5};

double timing::now(v8::Isolate* isolate) {
	auto time_since_epoch = time_point_cast<microseconds>(system_clock::now()).time_since_epoch().count();
	auto fuzz = clockRandomizer(rng);
	auto fuzzed_time = time_since_epoch + fuzz;
	return fuzzed_time;
}