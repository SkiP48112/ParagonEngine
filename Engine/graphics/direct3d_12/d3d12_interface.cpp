#include "app_common_headers.h"
#include "graphics\gr_platform_interface.h"
#include "d3d12_interface.h"
#include "d3d12_core.h"


void d3d12GetPlatformInterface(grPLATFORM_INTERFACE& platformInterface)
{
   platformInterface.Initialize = d3d12Initialize;
   platformInterface.Shutdown = d3d12Shutdown;
   platformInterface.Render = d3d12Render;
}