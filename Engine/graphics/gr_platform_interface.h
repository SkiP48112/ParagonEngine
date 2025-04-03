#pragma once

#include "app_common_headers.h"
#include "gr_renderer.h"
#include "platform/app_window.h"


struct grPLATFORM_INTERFACE
{
   bool(*Initialize)(void);
   void(*Shutdown)(void);
   void(*Render)(void);

   struct
   {
      grSURFACE(*Create)(appWINDOW);
      void(*Remove)(grSURFACE_ID);
      void(*Resize)(grSURFACE_ID, U32, U32);
      U32(*GetWidth)(grSURFACE_ID);
      U32(*GetHeight)(grSURFACE_ID);
      void(*Render)(grSURFACE_ID);
   } surface;
};