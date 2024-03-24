#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include <cstdint>
#include <vector>
#include <atomic>

template <typename key_t>
class hashtable {
	parlay::sequence<std::atomic<key_t>> table;
	std::vector<bool> exists;

	hashtable(const size_t size) {
		table = parlay::sequence<std::atomic<key_t>>::uninitialized(size);
		exists = std::vector<bool>(size, 0);
	}

	bool insert(key_t key) {
		// To be implemented
		return false;
	}

	bool remove(key_t key) {
		// To be implemented
		return false;
	}

	bool find(key_t key) {
		// To be implemented
		return false;
	}

	bool cas(size_t index, val_t& exp, val_t& upd) {
		return table[index].compare_exchange_strong(exp, upd);
	}

	size_t hash(const key_t& key, uint32_t seed) {
		const int len = sizeof(key_t);
		const uint64_t m = 0xc6a4a7935bd1e995;
		const int r = 47;

		uint64_t h = seed ^ (len * m);

		const uint64_t *data = (const uint64_t*)(&key);
		const uint64_t *end = data + (len / 8);

		while (data != end) {
			uint64_t k = *data++;

			k *= m;
			k ^= k >> r;
			k *= m;

			h ^= k;
			h *= m;
		}

		const unsigned char *data2 = (const unsigned char*)data;
		switch (len & 7) {
			case 7: h ^= (uint64_t)data2[6] << 48;
			case 6: h ^= (uint64_t)data2[5] << 40;
			case 5: h ^= (uint64_t)data2[4] << 32;
			case 4: h ^= (uint64_t)data2[3] << 24;
			case 3: h ^= (uint64_t)data2[2] << 16;
			case 2: h ^= (uint64_t)data2[1] << 8;
			case 1: h ^= (uint64_t)data2[0];
					h *= m;
		};

		h ^= h >> r;
		h *= m;
		h ^= h >> r;

		return h;
	}
}

#endif
