#pragma once

#include <cstdint>

namespace utils {
	template<unsigned char bytes>
	struct type_sizer {
		typedef typename type_sizer<bytes - 1>::type type;
	};

	template<>
	struct type_sizer<0> {
		typedef std::uint_fast8_t type;
	};

	template<>
	struct type_sizer<2> {
		typedef std::uint_fast16_t type;
	};
	template<>
	struct type_sizer<3> {
		typedef std::uint_fast32_t type;
	};
	template<>
	struct type_sizer<5> {
		typedef std::uint_fast64_t type;
	};
	template<>
	struct type_sizer<9> {
	};

	constexpr unsigned char byte_sizer(std::size_t number) {
		if (!number) return 0;
		unsigned char count = 0;
		do count++; while (number >>= sizeof(unsigned char));
		return count;
	}
}