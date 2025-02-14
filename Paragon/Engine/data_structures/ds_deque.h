#pragma once

#define USE_STR_DEQUE true


#ifdef USE_STR_DEQUE
	#include <deque>
	template<typename T>
	using dsDEQUE = std::deque<T>;
#endif // USE_STR_DEQUE

	// TODO: write my own implementation of stl contrainers