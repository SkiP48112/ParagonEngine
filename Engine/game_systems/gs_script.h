#pragma once

#include "gs_common.h"


struct gsSCRIPT_INIT_INFO
{
	gsSCRIPT_CREATOR scriptCreator;
};


gsSCRIPT_COMPONENT gsCreateScript(gsSCRIPT_INIT_INFO info, gsENTITY entity);
void gsRemoveScript(gsSCRIPT_COMPONENT component);
void gsUpdateScripts(float dt);