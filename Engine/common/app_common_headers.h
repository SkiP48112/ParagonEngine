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


#ifndef DISABLE_COPY
#define DISABLE_COPY(T)                         \
            explicit T(const T&) = delete;      \
            T& operator=(const T&) = delete;
#endif


#ifndef DISABLE_MOVE
#define DISABLE_MOVE(T)                         \
            explicit T(T&&) = delete;      \
            T& operator=(T&&) = delete;
#endif


#ifndef DISABLE_COPY_AND_MOVE
#define DISABLE_COPY_AND_MOVE(T) DISABLE_COPY(T) DISABLE_MOVE(T)
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