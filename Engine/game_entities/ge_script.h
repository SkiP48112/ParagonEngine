#pragma once

#include "ge_common.h"


struct geSCRIPT_INIT_INFO
{
	geSCRIPT_CREATOR scriptCreator;
};


geSCRIPT_COMPONENT geCreateScript(geSCRIPT_INIT_INFO info, geENTITY entity);
void geRemoveScript(geSCRIPT_COMPONENT component);
void geUpdateScripts(float dt);