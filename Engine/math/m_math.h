#pragma once

#include "app_common_headers.h"
#include "..\math\m_vector2.h"
#include "..\math\m_vector3.h"
#include "..\math\m_vector4.h"
#include "..\math\m_matr3.h"
#include "..\math\m_matr4.h"


template<typename T>
constexpr T mClamp(T value, T min, T max)
{
   return (value < min) ? min : (value > max) ? max : value;
}