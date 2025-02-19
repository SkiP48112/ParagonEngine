#pragma once

#include "ge_common.h"


struct geTRANSFORM_INIT_INFO
{
	F32 position[3]{};
	F32 rotation[4]{};
	F32 scale[3]{1.0f, 1.0f, 1.0f};
};


geTRANSFORM_COMPONENT geCreateTransform(geTRANSFORM_INIT_INFO info, geENTITY entity);
void geRemoveTrasnform(geTRANSFORM_COMPONENT component);