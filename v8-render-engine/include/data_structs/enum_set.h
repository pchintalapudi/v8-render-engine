#pragma once

#include <array>
#include <type_traits>
#include <bitset>

namespace data_structs {
	template<typename E>
	class EnumSet {
	public:
		EnumSet() = default;

		template<typename... Pack>
		explicit EnumSet(Pack... pack) {
			this->add(pack...);
		}

		template<typename ...Pack>
		EnumSet& add(Pack... pack) {
			std::array<E, sizeof...(Pack)> arr = { pack... };
			for (E e : arr) {
				this->bitfield[static_cast<std::size_t>(e)] = true;
			}
			return *this;
		}

		template<typename ...Pack>
		EnumSet& remove(Pack... pack) {
			std::array<E, sizeof...(Pack)> arr = { pack... };
			for (E e : arr) {
				this->bitfield[static_cast<std::size_t>(e)] = false;
			}
			return *this;
		}

		bool contains(E e) const {
			return this->bitfield[static_cast<std::size_t>(e)];
		}

		bool any(EnumSet<E> es) const {
			return (this->bitfield & es.bitfield).any();
		}

		template<typename ...Pack>
		bool any(Pack... pack) const {
			return any(EnumSet<E>(pack...));
		}

		bool all(EnumSet<E> es) const {
			return this->bitfield & es.bitfield == es.bitfield;
		}

		template<typename ...Pack>
		bool all(Pack... pack) const {
			return all(EnumSet<E>(pack...));
		}

		bool none(EnumSet<E> es) const {
			return !this->any(es);
		}

		template<typename ...Pack>
		bool none(Pack... pack) const {
			return !this->any(pack...);
		}

		template<typename ...Pack>
		void toggle(bool force, Pack... pack) {
			if (force) {
				this->add(pack...);
			}
			else {
				this->remove(pack...);
			}
		}

	private:
		std::bitset<static_cast<std::size_t>(E::__COUNT__)> bitfield;
	};
}