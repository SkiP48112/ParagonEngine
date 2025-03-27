#pragma once

#include "app_common_headers.h"
#include "gr_renderer.h"


struct grPLATFORM_INTERFACE
{
   bool(*Initialize)(void);
   void(*Shutdown)(void);
};