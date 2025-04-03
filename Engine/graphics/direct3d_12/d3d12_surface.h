#pragma once
#include "d3d12_common_headers.h"
#include "d3d12_resources.h"


struct d3d12RENDER_TARGET_DATA
{
   ID3D12Resource* resource = nullptr;
   d3d12DESCRIPTOR_HANDLE rtv{};
};


class d3d12SURFACE
{
public:
   explicit d3d12SURFACE(appWINDOW window)
      : window(window)
   {
      assert(window.GetHandle());
   }

#if USE_STL_VECTOR
   DISABLE_COPY(d3d12SURFACE);
   constexpr d3d12SURFACE(d3d12SURFACE&& o)
      : swapChain{ o.swapChain }, window{ o.window }, currentBackBufferIndex{ o.currentBackBufferIndex }
      , viewport{ o.viewport }, scissorRect{ o.scissorRect }, allowTearing{o.allowTearing}, presentFlags{o.presentFlags}
   {
      for (U32 i = 0; i < D3D12_FRAME_BUFFER_COUNT; ++i)
      {
         renderTargetData[i].resource = o.renderTargetData[i].resource;
         renderTargetData[i].rtv = o.renderTargetData[i].rtv;
      }

      o.Reset();
   }

   constexpr d3d12SURFACE& operator=(d3d12SURFACE&& o)
   {
      assert(this != &o);
      if (this != &o)
      {
         Release();
         Move(o);
      }
      return *this;
   }
#endif

   ~d3d12SURFACE()
   {
      Release();
   }

   void CreateSwapChain(IDXGIFactory7* factory, ID3D12CommandQueue* commandQueue, DXGI_FORMAT format);
   void Present() const;
   void Resize() const;

   constexpr U32 GetWidth() const
   {
      return (U32)viewport.Width;
   }

   constexpr U32 GetHeight() const
   {
      return (U32)viewport.Height;
   }

   constexpr ID3D12Resource* const GetBackBuffer() const
   {
      return renderTargetData[currentBackBufferIndex].resource;
   }

   constexpr D3D12_CPU_DESCRIPTOR_HANDLE GetRTV() const
   {
      return renderTargetData[currentBackBufferIndex].rtv.cpu;
   }

   constexpr const D3D12_VIEWPORT& GetViewport() const
   {
      return viewport;
   }

   constexpr const D3D12_RECT& GetScissorRect() const
   {
      return scissorRect;
   }

private:
   void Finalize();
   void Release();

#if USE_STL_VECTOR
   constexpr void Move(d3d12SURFACE& o)
   {
      swapChain = o.swapChain;
      for (U32 i = 0; i < D3D12_FRAME_BUFFER_COUNT; ++i)
      {
         renderTargetData[i] = o.renderTargetData[i];
      }

      window = o.window;
      currentBackBufferIndex = o.currentBackBufferIndex;
      allowTearing = o.allowTearing;
      presentFlags = o.presentFlags;
      viewport = o.viewport;
      scissorRect = o.scissorRect;

      o.Reset();
   }


   constexpr void Reset()
   {
      swapChain = nullptr;
      for (U32 i = 0; i < D3D12_FRAME_BUFFER_COUNT; ++i)
      {
         renderTargetData[i] = {};
      }

      window = {};
      currentBackBufferIndex = 0;
      allowTearing = 0;
      presentFlags = 0;
      viewport = {};
      scissorRect = {};
   }
#endif

private:
   IDXGISwapChain4* swapChain = nullptr;
   d3d12RENDER_TARGET_DATA renderTargetData[D3D12_FRAME_BUFFER_COUNT];
   appWINDOW window{};

   mutable U32 currentBackBufferIndex = 0;
   U32 allowTearing = 0;
   U32 presentFlags = 0;
   
   D3D12_VIEWPORT viewport{};
   D3D12_RECT scissorRect{};
};