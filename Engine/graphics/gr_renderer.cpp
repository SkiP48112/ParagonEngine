#include "gr_renderer.h"
#include "gr_platform_interface.h"
#include "direct3d_12\d3d12_interface.h"


namespace
{
   grPLATFORM_INTERFACE gfx;

   bool grSetPlatformInterface(grGRAPHICS_PLATFORM platform)
   {
      switch (platform)
      {
      case grGRAPHICS_PLATFORM::GP_DIRECT3D_12:
         d3d12GetPlatformInterface(gfx);
         break;
      default:
         return false;
      }

      return true;
   }
}


bool grInitialize(grGRAPHICS_PLATFORM platform)
{
   return grSetPlatformInterface(platform) && gfx.Initialize();
}


void grShutdown()
{
   gfx.Shutdown();
}