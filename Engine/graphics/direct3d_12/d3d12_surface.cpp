#include "d3d12_surface.h"
#include "d3d12_core.h"


namespace
{
   constexpr DXGI_FORMAT d3d12ToNonSRGB(DXGI_FORMAT format)
   {
      if (format == DXGI_FORMAT_R8G8B8A8_UNORM_SRGB)
      {
         return DXGI_FORMAT_R8G8B8A8_UNORM;
      }

      return format;
   }
}


void d3d12SURFACE::CreateSwapChain(IDXGIFactory7* factory, ID3D12CommandQueue* commandQueue, DXGI_FORMAT format)
{
   assert(factory && commandQueue);
   Release();

   if (SUCCEEDED(factory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(U32))) && allowTearing)
   {
      presentFlags = DXGI_PRESENT_ALLOW_TEARING;
   }

   DXGI_SWAP_CHAIN_DESC1 desc{};
   desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
   desc.BufferCount = D3D12_FRAME_BUFFER_COUNT;
   desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
   desc.Flags = allowTearing ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;
   desc.Format = d3d12ToNonSRGB(format);
   desc.Height = window.GetHeight();
   desc.Width = window.GetWidth();
   desc.SampleDesc.Count = 1;
   desc.SampleDesc.Quality = 0;
   desc.Scaling = DXGI_SCALING_STRETCH;
   desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
   desc.Stereo = false;

   IDXGISwapChain1* chain;
   HWND hwnd = (HWND)window.GetHandle();

   ASSERT_DX(factory->CreateSwapChainForHwnd(commandQueue, hwnd, &desc, nullptr, nullptr, &chain));
   ASSERT_DX(factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER));
   ASSERT_DX(chain->QueryInterface(IID_PPV_ARGS(&swapChain)));

   d3d12Release(chain);

   currentBackBufferIndex = swapChain->GetCurrentBackBufferIndex();
   for (U32 i = 0; i < D3D12_FRAME_BUFFER_COUNT; ++i)
   {
      renderTargetData[i].rtv = d3d12GetRTVHeap().Allocate();
   }

   Finalize();
}


void d3d12SURFACE::Finalize()
{
   for (U32 i = 0; i < D3D12_FRAME_BUFFER_COUNT; ++i)
   {
      d3d12RENDER_TARGET_DATA& data = renderTargetData[i];
      assert(!data.resource);

      ASSERT_DX(swapChain->GetBuffer(i, IID_PPV_ARGS(&data.resource)));

      D3D12_RENDER_TARGET_VIEW_DESC desc{};
      desc.Format = d3d12GetDefaultRenderTargetFormat();
      desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
      d3d12GetMainDevice()->CreateRenderTargetView(data.resource, &desc, data.rtv.cpu);
   }

   DXGI_SWAP_CHAIN_DESC desc;
   ASSERT_DX(swapChain->GetDesc(&desc));

   const U32 width = desc.BufferDesc.Width;
   const U32 height = desc.BufferDesc.Height;
   assert(window.GetWidth() == width && window.GetHeight() == height);

   // set viewport and scissor rect
   viewport.TopLeftX = 0.0f;
   viewport.TopLeftY = 0.0f;
   viewport.Width = (float)width;
   viewport.Height = (float)height;
   viewport.MinDepth = 0.0f;
   viewport.MaxDepth = 1.0f;

   scissorRect = { 0, 0, (S32)width, (S32)height };
}


void d3d12SURFACE::Present() const
{
   assert(swapChain);
   ASSERT_DX(swapChain->Present(0, presentFlags));

   currentBackBufferIndex = swapChain->GetCurrentBackBufferIndex();
}


void d3d12SURFACE::Resize() const
{

}


void d3d12SURFACE::Release()
{
   for (U32 i = 0; i < D3D12_FRAME_BUFFER_COUNT; ++i)
   {
      d3d12RENDER_TARGET_DATA& data = renderTargetData[i];
      d3d12Release(data.resource);
      d3d12GetRTVHeap().Free(data.rtv);
   }

   d3d12Release(swapChain);
}