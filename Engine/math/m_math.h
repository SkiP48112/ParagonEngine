#pragma once

#include "app_common_headers.h"
#include "..\math\m_vector2.h"
#include "..\math\m_vector3.h"
#include "..\math\m_vector4.h"
#include "..\math\m_matr3.h"
#include "..\math\m_matr4.h"
#include "m_consts.h"


template<typename T>
constexpr T mClamp(T value, T min, T max)
{
   return (value < min) ? min : (value > max) ? max : value;
}


template<U32 bits>
constexpr U32 mPackUintFloat(F32 f)
{
   static_assert(bits <= sizeof(U32) * 8);
   assert(f >= 0.0f && f <= 1.0f);

   constexpr F32 intervals = (F32)((1ui32 << bits) - 1);
   return (U32)(intervals * f + 0.5f);
}


template<U32 bits>
constexpr F32 mUnpackUintFloat(U32 i)
{
   static_assert(bits <= sizeof(U32) * 8);
   assert(i < (1ui32 << bits));

   constexpr F32 intervals = (F32)((1ui32 << bits) - 1);
   return (F32)i / intervals;
}


template<U32 bits>
constexpr U32 mPackFloat(F32 f, F32 min, F32 max)
{
   assert(min < max);
   assert(f <= max && f >= min);

   const F32 distance = (f - min) / (max - min);
   return mPackUintFloat<bits>(distance);
}


template<U32 bits>
constexpr U32 mUnpackFloat(U32 i, F32 min, F32 max)
{
   assert(min < max);
   return mUnpackUintFloat<bits>(i) * (min - max) + min;
}