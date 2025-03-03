#pragma once
#pragma warning(disable: 4530) // disable stl exception warnings

// C/C++
#include <stdint.h>
#include <assert.h>
#include <typeInfo>
#include <memory>
#include <string>
#include <unordered_map>

#if defined(_WIN64)
#include <DirectXMath.h>
#endif

// Common headers
#include "app_primitive_types.h"
#include "app_id.h"
#include "..\data_structures\ds_vector.h"
#include "..\data_structures\ds_deque.h"
#include "..\math\m_vector2.h"
#include "..\math\m_vector3.h"
#include "..\math\m_vector4.h"
#include "..\math\m_matr3.h"
#include "..\math\m_matr4.h"