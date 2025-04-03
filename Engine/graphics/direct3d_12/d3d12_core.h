#pragma once

#include "d3d12_common_headers.h"

class d3d12DESCRIPTOR_HEAP;

bool d3d12Initialize();
void d3d12Shutdown();


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

d3d12DESCRIPTOR_HEAP& d3d12GetRTVHeap();
d3d12DESCRIPTOR_HEAP& d3d12GetDSVHeap();
d3d12DESCRIPTOR_HEAP& d3d12GetSRVHeap();
d3d12DESCRIPTOR_HEAP& d3d12GetUAVHeap();

DXGI_FORMAT d3d12GetDefaultRenderTargetFormat();

U32 d3d12GetCurrentFrameIndex();
void d3d12SetDeferredReleasesFlag();

grSURFACE d3d12CreateSurface(appWINDOW window);
void d3d12RemoveSurface(grSURFACE_ID id);
void d3d12ResizeSurface(grSURFACE_ID id, U32 width, U32 height);
U32 d3d12GetSurfaceWidth(grSURFACE_ID id);
U32 d3d12GetSurfaceHeight(grSURFACE_ID id);
void d3d12RenderSurface(grSURFACE_ID id);