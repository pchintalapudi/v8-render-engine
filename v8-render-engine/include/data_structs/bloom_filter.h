#pragma once
#include <bitset>
#include <random>

namespace data_structs {

	//Taken from http://quinnftw.com/c++-bloom-filter/

	// A utility class which provides uniformly distributed random numbers seeded
	// with the hash function on a given input.  Useful for generating multiple
	// bloomfilter bit indexes for a key.
	template <typename T, int Size, typename Hash = std::hash<T>>
	struct Mixer {
		std::minstd_rand rng_;
		Mixer(const T& val) : rng_(Hash()(val)) {}
		std::size_t operator()() { return rng_() % Size; }
	};


	// A probabilistic space efficient data structure used for testing membership in
	// a set.
	// https://en.wikipedia.org/wiki/Bloom_filter
	template <typename Key, int Size, int NumHashes, typename Hash = std::hash<Key>>
	class BloomFilter {
	public:
		BloomFilter() = default;

		BloomFilter(const std::initializer_list<Key>& init) {
			for (const auto& key : init) {
				Add(key);
			}
		}

		constexpr int size() const { return Size; }

		void add(const Key& key) {
			Mixer<Key, Size, Hash> mixer(key);
			for (int i = 0; i < NumHashes; ++i) {
				bits_.set(mixer());
			}
		}

		bool contains(const Key& key) const {
			Mixer<Key, Size, Hash> mixer(key);
			for (int i = 0; i < NumHashes; ++i) {
				if (!bits_[mixer()]) return false;
			}
			return true;
		}

		void clear() {
			this->bits_.reset();
		}

	private:
		std::bitset<Size> bits_;
	};
}