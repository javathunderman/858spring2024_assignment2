#ifndef _HOTSPOTS_H_
#define _HOTSPOTS_H_

#include <atomic>
#include "parlay/primitives.h"
#include "parlay/sequence.h"

template <typename vertex, typename graph>
auto hotspots(parlay::sequence<vertex> U, const graph& G) {
	vertex n = G.size();

	// n indicates that the vertex has not been visited yet
	auto nearest = parlay::tabulate<std::atomic<vertex>>(n, [&] (long i) {return n;});

	// mark the set U as visited from self
	parlay::parallel_for(0, U.size(), [&] (long i) {
		nearest[U[i]] = U[i];
	});

	// Code taken from Parlay's parallel BFS implementation, with modifications
	parlay::parallel_for(0, U.size(), [&] (long i) {
		parlay::sequence<vertex> frontier = G[U[i]];
		parlay::sequence<parlay::sequence<vertex>> frontiers;
		while (frontier.size() > 0) {
			frontiers.push_back(frontier);

			// get out edges of the frontier and flatten
			auto out = flatten(map(frontier, [&] (vertex u) {return G[u];}));

			// keep the v that succeed in setting the visited array
			frontier = filter(out, [&] (auto&& v) {
				// expand the frontier by checking which don't have a hotspot yet, and use CAS to set it
				return (nearest[v] == n) && nearest[v].compare_exchange_strong(n, U[i]);
			});
		}
	});

	// convert from sequence of atomics to a regular sequence
	return parlay::tabulate(n, [&] (long i) {return nearest[i].load();});
}

#endif
