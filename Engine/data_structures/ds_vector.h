#pragma once

#include <algorithm>


#define USE_STL_VECTOR true


#ifdef USE_STL_VECTOR
	#include <vector>
	template<typename T>
	using dsVECTOR = std::vector<T>;


	template<typename T>
	void dsEraseUnordered(std::vector<T>& v, size_t index)
	{
		if (v.size() > 1)
		{
			std::iter_swap(v.begin() + index, v.end() - 1);
			v.pop_back();
			return;
		}

		v.clear();
	}
#endif

	// TODO: write my own implementation of stl contrainers