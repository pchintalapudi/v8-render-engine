#pragma once
#include <array>
#include "enum_set.h"
namespace data_structs {
	template<typename E, typename V>
	class enum_map_value_wrapper {
	public:

		enum_map_value_wrapper(EnumSet<E>& set_enums, E e, V& value_loc)
			:set_enums(set_enums), e(e), value_loc(value_loc) {}

		operator V() {
			return this->value_loc;
		}

		V& operator=(const V& value) {
			if (value != this->value_loc) {
				value_loc = value;
				this->set_enums.add(this->e);
			}
			return value;
		}

	private:
		EnumSet<E>& set_enums;
		E e;
		V& value_loc;
	};

	template<typename E, typename V>
	class EnumMap {
	public:
		enum_map_value_wrapper<E, V> operator[](E e) {
			return enum_map_value_wrapper<E, V>(this->set_enums, e, this->values[e]);
		}

		bool contains(E e) {
			return this->set_enums.contains(e);
		}
	private:
		std::array<V, static_cast<int>(E::COUNT)> values;
		EnumSet<E> set_enums;
	};
}