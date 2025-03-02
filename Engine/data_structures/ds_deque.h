#pragma once

#define USE_STL_DEQUE true

#ifdef USE_STL_DEQUE
	#include <deque>


	template<typename T>
	using dsDEQUE = std::deque<T>;
#endif

	// TODO: write my own implementation of stl contrainers