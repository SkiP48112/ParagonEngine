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


grSURFACE grCreateSurface(appWINDOW window)
{
   return gfx.surface.Create(window);
}


void grRemoveSurface(grSURFACE_ID id)
{
   assert(idIsValid(id));
   gfx.surface.Remove(id);
}


void grSURFACE::Resize(U32 width, U32 height) const
{
   assert(IsValid());
   gfx.surface.Resize(id, width, height);
}


U32 grSURFACE::GetWidth() const
{
   assert(IsValid());
   return gfx.surface.GetWidth(id);
}


U32 grSURFACE::GetHeight() const
{
   assert(IsValid());
   return gfx.surface.GetHeight(id);
}


void grSURFACE::Render() const
{
   assert(IsValid());
   gfx.surface.Render(id);
}