#include "d3d12_core.h"


using namespace Microsoft::WRL;


namespace
{
   struct d3d12COMMAND_FRAME
   {
      ID3D12CommandAllocator* commandAllocator = nullptr;
      U64 fenceValue = 0;


      void Wait(HANDLE fenceEvent, ID3D12Fence1* fence)
      {
         assert(fence && fenceEvent);

         // NOTE: If the current fence value is less than "fenceValue"
         //       then we know that's the GPU has not finished executing commands
         if (fence->GetCompletedValue() < fenceValue)
         {
            HRESULT hResult = S_OK;
            ASSERT_DX(hResult = fence->SetEventOnCompletion(fenceValue, fenceEvent));
            if (FAILED(hResult))
            {
               return;
            }

            WaitForSingleObject(fenceEvent, INFINITE);
         }
      }


      void Release()
      {
         d3d12Release(commandAllocator);
      }
   };


   class d3d12COMMAND
   {
   public:
      d3d12COMMAND() = default;
      DISABLE_COPY_AND_MOVE(d3d12COMMAND);

      explicit d3d12COMMAND(ID3D12Device8* const device, D3D12_COMMAND_LIST_TYPE type)
      {
         HRESULT hResult = S_OK;
         
         hResult = CreateCommandQueue(device, type);
         if (FAILED(hResult))
         {
            Release();
            return;
         }

         hResult = CreateCommandFrames(device, type);
         if (FAILED(hResult))
         {
            Release();
            return;
         }

         hResult = CreateCommandList(device, type);
         if (FAILED(hResult))
         {
            Release();
            return;
         }

         hResult = CreateFence(device, type);
         if (FAILED(hResult))
         {
            Release();
            return;
         }
         
         fenceEvent = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
         assert(fenceEvent);
      }


      ~d3d12COMMAND()
      {
         assert(!commandQueue && !commandList && !fence);
      }


      constexpr ID3D12CommandQueue* const GetCommandQueue() const
      {
         return commandQueue;
      }


      constexpr ID3D12GraphicsCommandList6* const GetCommandList() const
      {
         return commandList;
      }


      constexpr U32 GetFrameIndex() const
      {
         return frameIndex;
      }


      void BeginFrame()
      {
         d3d12COMMAND_FRAME& frame = commandFrames[frameIndex];
         frame.Wait(fenceEvent, fence);

         ASSERT_DX(frame.commandAllocator->Reset());
         ASSERT_DX(commandList->Reset(frame.commandAllocator, nullptr));
      }


      void EndFrame()
      {
         ASSERT_DX(commandList->Close());
         ID3D12CommandList* const commandLists[]{ commandList };
         commandQueue->ExecuteCommandLists(_countof(commandLists), &commandLists[0]);

         U64& pFenceValue = fenceValue;
         pFenceValue++;

         d3d12COMMAND_FRAME& pFrame = commandFrames[frameIndex];
         pFrame.fenceValue = pFenceValue;

         commandQueue->Signal(fence, fenceValue);
         frameIndex = (frameIndex + 1) % D3D12_FRAME_BUFFER_COUNT;
      }


      void Release()
      {
         Flush();
         d3d12Release(fence);

         fenceValue = 0;
         CloseHandle(fenceEvent);
         fenceEvent = nullptr;

         d3d12Release(commandQueue);
         d3d12Release(commandList);

         for (U32 i = 0; i < D3D12_FRAME_BUFFER_COUNT; ++i)
         {
            commandFrames[i].Release();
         }
      }

   private:
      HRESULT CreateCommandQueue(ID3D12Device8* const device, D3D12_COMMAND_LIST_TYPE type)
      {
         HRESULT hResult = S_OK;

         D3D12_COMMAND_QUEUE_DESC desc;
         desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
         desc.NodeMask = 0;
         desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
         desc.Type = type;

         ASSERT_DX(device->CreateCommandQueue(&desc, IID_PPV_ARGS(&commandQueue)));
         if (FAILED(hResult))
         {
            return hResult;
         }

         NAME_D3D12_OBJECT(commandQueue,
            type == D3D12_COMMAND_LIST_TYPE_DIRECT ?
            L"GFX Command Queue" :
            type == D3D12_COMMAND_LIST_TYPE_COMPUTE ?
            L"Compute Command Queue" : L"Command Queue");

         return hResult;
      }


      HRESULT CreateCommandFrames(ID3D12Device8* const device, D3D12_COMMAND_LIST_TYPE type)
      {
         HRESULT hResult = S_OK;
         for (U32 i = 0; i < D3D12_FRAME_BUFFER_COUNT; ++i)
         {
            d3d12COMMAND_FRAME& frame = commandFrames[i];
            ASSERT_DX(hResult = device->CreateCommandAllocator(type, IID_PPV_ARGS(&frame.commandAllocator)));
            if (FAILED(hResult))
            {
               return hResult;
            }

            NAME_D3D12_INDEXED_OBJECT(frame.commandAllocator, i,
               type == D3D12_COMMAND_LIST_TYPE_DIRECT ?
               L"GFX Command Allocator" :
               type == D3D12_COMMAND_LIST_TYPE_COMPUTE ?
               L"Compute Command Allocator" : L"Command Allocator");
         }

         return hResult;
      }


      HRESULT CreateCommandList(ID3D12Device* const device, D3D12_COMMAND_LIST_TYPE type)
      {
         HRESULT hResult = S_OK;

         ASSERT_DX(hResult = device->CreateCommandList(0, type, commandFrames[0].commandAllocator, nullptr, IID_PPV_ARGS(&commandList)));
         if (FAILED(hResult))
         {
            return hResult;
         }

         ASSERT_DX(commandList->Close());
         NAME_D3D12_OBJECT(commandList,
            type == D3D12_COMMAND_LIST_TYPE_DIRECT ?
            L"GFX Command List" :
            type == D3D12_COMMAND_LIST_TYPE_COMPUTE ?
            L"Compute Command List" : L"Command List");

         return hResult;
      }


      HRESULT CreateFence(ID3D12Device8* const device, D3D12_COMMAND_LIST_TYPE type)
      {
         HRESULT hResult = S_OK;

         ASSERT_DX(hResult = device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));
         if (FAILED(hResult))
         {
            return hResult;
         }

         NAME_D3D12_OBJECT(fence, L"D3D12 Fence");
         return hResult;
      }


      void Flush()
      {
         for (U32 i = 0; i < D3D12_FRAME_BUFFER_COUNT; ++i)
         {
            commandFrames[i].Wait(fenceEvent, fence);
         }
         
         frameIndex = 0;
      }

   private:
      ID3D12CommandQueue* commandQueue = nullptr;
      ID3D12GraphicsCommandList6* commandList = nullptr;
      ID3D12Fence1* fence = nullptr;
      HANDLE fenceEvent = nullptr;

      d3d12COMMAND_FRAME commandFrames[D3D12_FRAME_BUFFER_COUNT];

      U32 frameIndex = 0;
      U64 fenceValue = 0;
   };


   ID3D12Device8* d3d12MainDevice = nullptr;
   IDXGIFactory7* d3d12DXGIFactory = nullptr;
   d3d12COMMAND gfxCommand;

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

   new (&gfxCommand) d3d12COMMAND(d3d12MainDevice, D3D12_COMMAND_LIST_TYPE_DIRECT);
   if (!gfxCommand.GetCommandQueue())
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
   gfxCommand.Release();
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


// NOTE: Wait for the GPU to finish with the command allocator and
//       reset the allocator once the GPU is done.
//       Tgis frees the memory that was used to store commands.
void d3d12Render()
{
   gfxCommand.BeginFrame();
   ID3D12GraphicsCommandList6* commandList = gfxCommand.GetCommandList();

   gfxCommand.EndFrame();
}