#pragma once

#include "common_headers.h"


#if defined(_WIN64)
	using mMATR4  = DirectX::XMFLOAT4X4;
	using mMATR4A = DirectX::XMFLOAT4X4A;
#endif

	// TODO: write my own implementation of matrices