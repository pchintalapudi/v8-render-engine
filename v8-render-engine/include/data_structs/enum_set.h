#pragma once

#include "utils/byte_sizer.h"
namespace data_structs {
	template<enum E>
	class EnumSet {
	public:
		EnumSet<E> add(E e) {
			this->field |= (1 << static_cast<int>(e));
			return this;
		}

		EnumSet<E> remove(E e) {
			this->field &= ~(1 << static_cast<int>(e));
			return this;
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
		utils::type_sizer<utils::byte_sizer(static_cast<int>(E::COUNT))>::type field;
	};
}