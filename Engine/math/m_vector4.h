#pragma once

#include "common_headers.h"


#if defined(_WIN64)
	using mVECTOR4     = DirectX::XMFLOAT4;
	using mVECTOR4A    = DirectX::XMFLOAT4A;
	using mVECTOR4_U32 = DirectX::XMUINT4;
	using mVECTOR4_S32 = DirectX::XMINT4;
#endif

	// TODO: write my own implementation of matrices