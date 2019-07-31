#pragma once
#include <type_traits>
#include <set>
#include "enum_set.h"

namespace data_structs {
	template<typename E>
	using fast_set = std::conditional<std::is_enum<E>, EnumSet<E>, std::set<E>>;
}