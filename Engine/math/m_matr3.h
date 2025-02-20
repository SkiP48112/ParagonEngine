#pragma once

#include "app_common_headers.h"


#if defined(_WIN64)
	using mMATR3 = DirectX::XMFLOAT3X3;
	// DirectX math library doesn't have aligned 3x3 matrices for a reason
#endif

	// TODO: write my own implementation of matrices