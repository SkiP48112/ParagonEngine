#pragma once
#include <stdint.h>

using U64 = uint64_t;
using U32 = uint32_t;
using U16 = uint16_t;
using U8  = uint8_t;

using S64 = int64_t;
using S32 = int32_t;
using S16 = int16_t;
using S8  = int8_t;

using F32 = float;

constexpr U64 U64_INVALID_ID{ 0xffff'ffff'ffff'ffffui64};
constexpr U32 U32_INVALID_ID{ 0xffff'ffffui32};
constexpr U16 U16_INVALID_ID{ 0xffffui16};
constexpr U8  U8_INVALID_ID { 0xffui8};