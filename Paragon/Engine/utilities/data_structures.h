#pragma once

#define USE_STL_VECTOR true
#define USE_STR_DEQUE true


#ifdef USE_STL_VECTOR
	#include <vector>
	template<typename T>
	using dsVECTOR = std::vector<T>;
#endif // USE_STL_VECTOR

#ifdef USE_STR_DEQUE
	#include <deque>
	template<typename T>
	using dsDEQUE = std::deque<T>;
#endif // USE_STR_DEQUE


// TODO: write my own implementation of stl contrainers