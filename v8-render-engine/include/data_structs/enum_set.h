#pragma once

#include <array>
#include <type_traits>

namespace data_structs {
	template<typename Enum>
	class EnumSet {
	public:
		EnumSet() = default;

		template<typename... Pack>
		explicit EnumSet(Pack... pack) {
			this->add(pack...);
		}

		template<typename ...Pack>
		EnumSet& add(Pack... pack) {
			std::array<Enum, sizeof...(Pack)> arr = { ...pack };
			for (Enum e of arr) {
				this->bitfield |= static_cast<std::underlying_type<Enum>::type>(e);
			}
			return *this;
		}

		template<typename ...Pack>
		EnumSet& remove(Pack... pack) {
			std::array<Enum, sizeof...(Pack)> arr = { ...pack };
			for (Enum e of arr) {
				this->bitfield &= ~static_cast<std::underlying_type<Enum>::type>(e);
			}
			return *this;
		}

		bool contains(Enum e) {
			return this->bitfield & static_cast<std::underlying_type<Enum>::type>(e);
		}

		bool any(EnumSet<Enum> es) {
			return this->bitfield & es.bitfield;
		}

		template<typename ...Pack>
		bool any(Pack... pack) {
			return any(EnumSet<Enum>(pack...));
		}

		bool all(EnumSet<Enum> es) {
			return this->bitfield & es.bitfield == es.bitfield;
		}

		template<typename ...Pack>
		bool all(Pack... pack) {
			return all(EnumSet<Enum>(pack...));
		}

		bool none(EnumSet<Enum> es) {
			return !this->any(es);
		}

		template<typename ...Pack>
		bool none(Pack... pack) {
			return !this->any(pack...);
		}

		bool operator==(const EnumSet<Enum>& other) {
			return other.bitfield == this->bitfield;
		}

		bool operator<(const EnumSet<Enum>& other) {
			return this->bitfield < other.bitfield;
		}

	private:
		std::underlying_type<Enum>::type bitfield = 0;
	};
}