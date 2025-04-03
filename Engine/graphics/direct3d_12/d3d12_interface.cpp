#include "app_common_headers.h"
#include "graphics\gr_platform_interface.h"
#include "d3d12_interface.h"
#include "d3d12_core.h"


void d3d12GetPlatformInterface(grPLATFORM_INTERFACE& platformInterface)
{
   platformInterface.Initialize = d3d12Initialize;
   platformInterface.Shutdown = d3d12Shutdown;

   platformInterface.surface.Create = d3d12CreateSurface;
   platformInterface.surface.Remove = d3d12RemoveSurface;
   platformInterface.surface.Resize = d3d12ResizeSurface;
   platformInterface.surface.GetWidth = d3d12GetSurfaceWidth;
   platformInterface.surface.GetHeight = d3d12GetSurfaceHeight;
   platformInterface.surface.Render = d3d12RenderSurface;
}