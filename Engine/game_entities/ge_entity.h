#pragma once

#include "ge_common.h"


#define INIT_INFO(COMPONENT) struct ge##COMPONENT##_##INIT_INFO
	
	INIT_INFO(TRANSFORM);
	INIT_INFO(SCRIPT);

#undef INIT_INFO


struct geENTITY_INFO
{
	geTRANSFORM_INIT_INFO* transform{ nullptr };
	geSCRIPT_INIT_INFO* script{ nullptr };
};


geENTITY geCreateGameEntity(geENTITY_INFO info);
void geRemoveGameEntity(geENTITY_ID id);
bool geIsAlive(geENTITY_ID id);