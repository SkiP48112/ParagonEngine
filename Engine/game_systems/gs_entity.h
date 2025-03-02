#pragma once

#include "gs_common.h"


#define INIT_INFO(COMPONENT) struct gs##COMPONENT##_##INIT_INFO
	
	INIT_INFO(TRANSFORM);
	INIT_INFO(SCRIPT);

#undef INIT_INFO


struct gsENTITY_INFO
{
	gsTRANSFORM_INIT_INFO* transform = nullptr;
	gsSCRIPT_INIT_INFO* script = nullptr;
};


gsENTITY gsCreateGameEntity(gsENTITY_INFO info);
void gsRemoveGameEntity(gsENTITY_ID id);
bool gsIsAlive(gsENTITY_ID id);