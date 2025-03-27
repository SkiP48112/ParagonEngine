#pragma once

#include "app_common_headers.h"
#include "..\platform\app_window.h"


class grSURFACE
{

};


struct grRENDER_SURFACE
{
   appWINDOW window;
   grSURFACE surface;
};


enum class grGRAPHICS_PLATFORM : U32
{
   GP_DIRECT3D_12 = 0,
};


bool grInitialize(grGRAPHICS_PLATFORM platform);
void grShutdown();

void grRender();