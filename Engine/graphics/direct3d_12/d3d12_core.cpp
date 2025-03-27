#include "d3d12_core.h"

namespace
{
   ID3D12Device9* d3d12MainDevice;
}


bool d3d12Initialize()
{
   // TODO: determine which graphics card should be used
   //       determina what is the maximum feature level that is supported
   //       create a ID3D12Device (virtual representation of graphics card)

   return true;
}


void d3d12Shutdown()
{

}