#pragma once

#define USE_STL_STRING true

#ifdef USE_STL_STRING
#include <string>

using dsSTRING = std::string;
#endif

// TODO: write my own implementation of stl contrainers