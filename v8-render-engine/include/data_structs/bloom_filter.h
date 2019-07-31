#pragma once
namespace data_structs {
	template<typename size_type>
	class BloomFilter {
	public:

		BloomFilter<size_type>& operator +=(size_type hash) {
			this->filter |= hash;
			return *this;
		}

		bool contains(size_type hash) const {
			return (hash & this->filter) == hash;
		}

		void clear() {
			this->filter = 0;
		}

	private:
		size_type filter;
	};
}