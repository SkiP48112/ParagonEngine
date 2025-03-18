#pragma once
#pragma warning(disable: 4530) // disable stl exception warnings

// C/C++
#include <stdint.h>
#include <assert.h>
#include <typeInfo>
#include <memory>
#include <unordered_map>

#if defined(_WIN64)
#include <DirectXMath.h>
#endif

// Common headers
#include "app_primitive_types.h"
#include "app_id.h"
#include "..\data_structures\ds_vector.h"
#include "..\data_structures\ds_deque.h"
#include "..\data_structures\ds_string.h"
#include "..\math\m_math.h"


#ifdef _DEBUG
#define DEBUG_OP(x) x
#else
#define DEBUG_OP(x) (void(0))
#endif