#pragma once

#include "d3d12_common_headers.h"


bool d3d12Initialize();
void d3d12Shutdown();
void d3d12Render();


template<typename T>
constexpr void d3d12Release(T*& resource)
{
   if (resource)
   {
      resource->Release();
      resource = nullptr;
   }
}