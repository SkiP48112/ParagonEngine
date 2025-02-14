#pragma once

#include "components_common.h"


#define INIT_INFO(COMPONENT) struct ge##COMPONENT##_##INIT_INFO
	INIT_INFO(TRANSFORM);
#undef INIT_INFO


struct geENTITY_INFO
{
	geTRANSFORM_INIT_INFO* transform{ nullptr };
};


geENTITY geCreateGameEntity(const geENTITY_INFO info);
void geRemoveGameEntity(geENTITY entity);
bool geIsAlive(geENTITY entity);