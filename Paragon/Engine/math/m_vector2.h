#pragma once

#include "common_headers.h"


#if defined(_WIN64)
	using mVECTOR2     = DirectX::XMFLOAT2;
	using mVECTOR2A    = DirectX::XMFLOAT2A;
	using mVECTOR2_U32 = DirectX::XMUINT2;
	using mVECTOR2_S32 = DirectX::XMINT2;
#endif

	// TODO: write my own implementation of vectors