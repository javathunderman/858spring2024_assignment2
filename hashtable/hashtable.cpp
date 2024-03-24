#include <iostream>
#include <string>

#include "hashtable.h"

int main(int argc, char **argv) {
	auto usage = "hash_map <n>";
	if (argc != 2) std::cout << usage << std::endl;
	else {
		long n;
		try { n = std::stol(argv[1]); }
		catch (...) { std::cout << usage << std::endl; return 1; }

		parlay::random_generator gen(0);
		std::uniform_int_distribution<long> dis(0, n);

		// generate n random keys
		auto pairs = parlay::tabulate(n, [&] (long i) {
				auto r = gen[i];
				return std::pair<long, long>{dis(r),i};});

		parlay::sequence<long> keys;
		parlay::internal::timer t("Time");
		for (int i = 0; i < 3; i++) {
			hash_map<long, long> m(n);
			t.next("construct");
			parlay::for_each(pairs, [&] (auto p) {
					m.insert(p.first, p.second);});
			t.next("insert");
			parlay::for_each(pairs, [&] (auto p) {
					m.find(p.second);});
			t.next("find");
		}

		std::cout << "num unique keys: " << keys.size() << std::endl;
	}
}
