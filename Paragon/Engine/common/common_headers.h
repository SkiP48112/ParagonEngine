#pragma once
#pragma warning(disable: 4530) // disable stl exception warnings

// C/C++
#include <stdint.h>
#include <assert.h>
#include <typeInfo>

#if defined(_WIN64)
#include <DirectXMath.h>
#endif

// Common headers
#include "primitive_types.h"
#include "..\utilities\data_structures.h"
#include "..\utilities\math_types.h"