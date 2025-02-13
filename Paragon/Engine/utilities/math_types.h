#pragma once

#include "common_headers.h"

	constexpr float MATH_PI = 3.1415926535897932384626433832795f;
	constexpr float MATH_EPSILON = 1e-5f;

#if defined(_WIN64)
	using mathVECTOR_2 = DirectX::XMFLOAT2;
	using mathVECTOR_3 = DirectX::XMFLOAT3;
	using mathVECTOR_4 = DirectX::XMFLOAT4;

	using mathVECTOR_2_A = DirectX::XMFLOAT2A;
	using mathVECTOR_3_A = DirectX::XMFLOAT3A;
	using mathVECTOR_4_A = DirectX::XMFLOAT4A;

	using mathU32_VECTOR_2 = DirectX::XMUINT2;
	using mathU32_VECTOR_3 = DirectX::XMUINT3;
	using mathU32_VECTOR_4 = DirectX::XMUINT4;

	using mathS32_VECTOR_2 = DirectX::XMINT2;
	using mathS32_VECTOR_3 = DirectX::XMINT3;
	using mathS32_VECTOR_4 = DirectX::XMINT4;

	using mathMATRIX_3x3 = DirectX::XMFLOAT3X3;
	using mathMATRIX_4x4 = DirectX::XMFLOAT4X4;

	// DirectX math library doesn't have aligned 3x3 matrices for a reason
	using mathMATRIX_4x4_A = DirectX::XMFLOAT4X4A;
#endif