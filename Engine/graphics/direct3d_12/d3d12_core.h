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


void d3d12DefferedReleaseInternal(IUnknown* resource);


template<typename T>
constexpr void d3d12DeferredRelease(T*& resource)
{
   if (resource)
   {
      d3d12DefferedReleaseInternal(resource);
      resource = nullptr;
   }
}


ID3D12Device* const d3d12GetMainDevice();

U32 d3d12GetCurrentFrameIndex();
void d3d12SetDeferredReleasesFlag();