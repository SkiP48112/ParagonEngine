#pragma once

#include "common_headers.h"

namespace paragon::math
{
	constexpr float PI = 3.1415926535897932384626433832795f;
	constexpr float EPSILON = 1e-5f;

#if defined(_WIN64)
	using Vector2 = DirectX::XMFLOAT2;
	using Vector3 = DirectX::XMFLOAT3;
	using Vector4 = DirectX::XMFLOAT4;

	using Vector2A = DirectX::XMFLOAT2A;
	using Vector3A = DirectX::XMFLOAT3A;
	using Vector4A = DirectX::XMFLOAT4A;

	using U32Vector2 = DirectX::XMUINT2;
	using U32Vector3 = DirectX::XMUINT3;
	using U32Vector4 = DirectX::XMUINT4;

	using S32Vector2 = DirectX::XMINT2;
	using S32Vector3 = DirectX::XMINT3;
	using S32Vector4 = DirectX::XMINT4;

	using Matrix3x3 = DirectX::XMFLOAT3X3;
	using Matrix4x4 = DirectX::XMFLOAT4X4;

	// DirectX math library doesn't have aligned 3x3 matrices for a reason
	using Matrix4x4A = DirectX::XMFLOAT4X4A;
#endif
}