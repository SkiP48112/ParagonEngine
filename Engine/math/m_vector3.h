#pragma once

#include "app_common_headers.h"


#if defined(_WIN64)
	using mVECTOR3     = DirectX::XMFLOAT3;
	using mVECTOR3A	   = DirectX::XMFLOAT3A;
	using mVECTOR3_U32 = DirectX::XMUINT3;
	using mVECTOR3_S32 = DirectX::XMINT3;
#endif

	// TODO: write my own implementation of matrices