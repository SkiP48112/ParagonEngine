#pragma once

#define USE_STL_VECTOR true
#define USE_STR_DEQUE true


#ifdef USE_STL_VECTOR
	#include <vector>
	namespace paragon::utl 
	{
		template<typename T>
		using Vector = std::vector<T>;
	}
#endif // USE_STL_VECTOR

#ifdef USE_STR_DEQUE
	#include <deque>
	namespace paragon::utl
	{
		template<typename T>
		using Deque = std::deque<T>;
	}
#endif // USE_STR_DEQUE


namespace paragon::utl
{
	// TODO: write my own implementation of stl contrainers
}