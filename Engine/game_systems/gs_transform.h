#pragma once

#include "gs_common.h"


struct gsTRANSFORM_INIT_INFO
{
	F32 position[3]{};
	F32 rotation[4]{};
	F32 scale[3]{1.0f, 1.0f, 1.0f};
};


gsTRANSFORM_COMPONENT gsCreateTransform(gsTRANSFORM_INIT_INFO info, gsENTITY entity);
void gsRemoveTrasnform(gsTRANSFORM_COMPONENT component);