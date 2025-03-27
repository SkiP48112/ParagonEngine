#include "d3d12_core.h"


using namespace Microsoft::WRL;


namespace
{
   ID3D12Device8* d3d12MainDevice = nullptr;
   IDXGIFactory7* d3d12DXGIFactory = nullptr;

   constexpr D3D_FEATURE_LEVEL d3d12MinimunFeatureLevel = D3D_FEATURE_LEVEL_11_0;


   bool d3d12FailedInit()
   {
      d3d12Shutdown();
      return false;
   }


   IDXGIAdapter4* d3d12DetermineMainAdapter()
   {
      IDXGIAdapter4* adapter = nullptr;
      for (U32 i = 0; d3d12DXGIFactory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter)) != DXGI_ERROR_NOT_FOUND; ++i)
      {
         if (SUCCEEDED(D3D12CreateDevice(adapter, d3d12MinimunFeatureLevel, __uuidof(ID3D12Device), nullptr)))
         {
            return adapter;
         }

         d3d12Release(adapter);
      }

      return nullptr;
   }


   D3D_FEATURE_LEVEL d3d12GetMaxFeatureLevel(IDXGIAdapter4* adapter)
   {
      constexpr D3D_FEATURE_LEVEL featureLevels[4]{
         D3D_FEATURE_LEVEL_11_0,
         D3D_FEATURE_LEVEL_11_1,
         D3D_FEATURE_LEVEL_12_0,
         D3D_FEATURE_LEVEL_12_1,
      };

      D3D12_FEATURE_DATA_FEATURE_LEVELS featureLevelInfo;
      featureLevelInfo.NumFeatureLevels = _countof(featureLevels);
      featureLevelInfo.pFeatureLevelsRequested = featureLevels;

      ComPtr<ID3D12Device> device;
      ASSERT_DX(D3D12CreateDevice(adapter, d3d12MinimunFeatureLevel, IID_PPV_ARGS(&device)));
      ASSERT_DX(device->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS, &featureLevelInfo, sizeof(featureLevelInfo)));

      return featureLevelInfo.MaxSupportedFeatureLevel;
   }
}


bool d3d12Initialize()
{
   if (d3d12MainDevice)
   {
      d3d12Shutdown();
   }

   U32 dxgiFactoryFlags = 0;
#ifdef _DEBUG
   // Enable debugging layer. Requires "Graphics Tools" optional feature.
   {
      ComPtr<ID3D12Debug3> debugInterface;
      ASSERT_DX(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface)));
      debugInterface->EnableDebugLayer();

      dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
   }
#endif

   HRESULT hresult = S_OK;
   ASSERT_DX(hresult = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&d3d12DXGIFactory)));
   if (FAILED(hresult))
   {
      return d3d12FailedInit();
   }

   ComPtr<IDXGIAdapter4> mainAdapter;
   mainAdapter.Attach(d3d12DetermineMainAdapter());
   if (!mainAdapter)
   {
      return d3d12FailedInit();
   }

   D3D_FEATURE_LEVEL maxFeatureLevel = d3d12GetMaxFeatureLevel(mainAdapter.Get());
   assert(maxFeatureLevel >= d3d12MinimunFeatureLevel);
   if (maxFeatureLevel < d3d12MinimunFeatureLevel)
   {
      return d3d12FailedInit();
   }

   ASSERT_DX(hresult = D3D12CreateDevice(mainAdapter.Get(), maxFeatureLevel, IID_PPV_ARGS(&d3d12MainDevice)));
   if (FAILED(hresult))
   {
      return d3d12FailedInit();
   }

   NAME_D3D12_OBJECT(d3d12MainDevice, L"Main D3D12 Device");

#ifdef _DEBUG
   {
      ComPtr<ID3D12InfoQueue> infoQueue;
      ASSERT_DX(d3d12MainDevice->QueryInterface(IID_PPV_ARGS(&infoQueue)));
   
      infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
      infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
      infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
   }
#endif

   return true;
}


void d3d12Shutdown()
{
   d3d12Release(d3d12DXGIFactory);

#ifdef _DEBUG
   {
      ComPtr<ID3D12InfoQueue> infoQueue;
      ASSERT_DX(d3d12MainDevice->QueryInterface(IID_PPV_ARGS(&infoQueue)));

      infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, false);
      infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, false);
      infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, false);
   }

   ComPtr<ID3D12DebugDevice2> debugDevice;
   ASSERT_DX(d3d12MainDevice->QueryInterface(IID_PPV_ARGS(&debugDevice)));
   d3d12Release(d3d12MainDevice);
   ASSERT_DX(debugDevice->ReportLiveDeviceObjects(D3D12_RLDO_SUMMARY | D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL));
#endif

   d3d12Release(d3d12MainDevice);
}