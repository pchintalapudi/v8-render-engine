#pragma once

#include "utils/byte_sizer.h"
namespace data_structs {
	template<typename E>
	class EnumSet {
	public:

		EnumSet() = default;

		template<typename ...Pack>
		EnumSet(Pack... p) {
			this->add(p);
		}

		template<typename ...Pack>
		EnumSet<E>& add(Pack... p) {
			std::array<E, sizeof...(p)> args = { p... };
			for (E e : args) this->field |= (1 << static_cast<integer>(e));
			return *this;
		}

		EnumSet<E>& add(const EnumSet<E>& other) {
			this->field |= other.field;
			return *this;
		}

		template<typename ...Pack>
		EnumSet<E>& remove(Pack... p) {
			std::array<E, sizeof...(p)> args = { p... };
			for (E e : args) this->field &= ~(1 << static_cast<integer>(e));
			return *this;
		}

		EnumSet<E>& remove(const EnumSet<E>& other) {
			this->field &= ~other.field;
			return *this;
		}

		bool contains(E e) const {
			return !!(this->field & (1 << static_cast<int>(e)));
		}

		bool operator==(const EnumSet<E>& other) const { return field == other.field; }

		bool operator!=(const EnumSet<E>& other) const { return field != other.field; }

		bool operator<(const EnumSet<E>& other) const {
			return field < other.field;
		}

		bool operator<=(const EnumSet<E>& other) const {
			return field <= other.field;
		}

		bool operator>(const EnumSet<E>& other) const {
			return field > other.field;
		}

		bool operator>=(const EnumSet<E>& other) const {
			return field >= other.field;
		}

	private:
		//TODO: Use std::underlying_type instead of int
		typedef int integer;
		integer field;
	};
}