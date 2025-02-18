#pragma once

#define USE_STL_VECTOR true

#ifdef USE_STL_VECTOR
	#include <vector>
	template<typename T>
	using dsVECTOR = std::vector<T>;
#endif // USE_STL_VECTOR

	// TODO: write my own implementation of stl contrainers