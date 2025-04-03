#include "d3d12_resources.h"
#include "d3d12_core.h"


// -------------------- d3d12DESCRIPTOR_HEAP --------------------


bool d3d12DESCRIPTOR_HEAP::Initialize(U32 capacity, bool isShaderVisible)
{
   std::lock_guard lock(mutex);

   assert(capacity && capacity < D3D12_MAX_SHADER_VISIBLE_DESCRIPTOR_HEAP_SIZE_TIER_2);
   assert(!(type == D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER && capacity > D3D12_MAX_SHADER_VISIBLE_SAMPLER_HEAP_SIZE));
   if (type == D3D12_DESCRIPTOR_HEAP_TYPE_DSV || type == D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
   {
      isShaderVisible = false;
   }

   Release();

   ID3D12Device* const device = d3d12GetMainDevice();
   assert(device);

   D3D12_DESCRIPTOR_HEAP_DESC desc;
   desc.Flags = isShaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
   desc.NumDescriptors = capacity;
   desc.Type = type;
   desc.NodeMask = 0;

   HRESULT hResult = S_OK;
   ASSERT_DX(hResult = device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap)));
   if (FAILED(hResult))
   {
      return false;
   }

   freeHandles = std::move(std::make_unique<U32[]>(capacity));
   this->capacity = capacity;
   size = 0;

   for (U32 i = 0; i < capacity; ++i)
   {
      freeHandles[i] = i;
   }

#ifdef _DEBUG
   for (U32 i = 0; i < D3D12_FRAME_BUFFER_COUNT; ++i)
   {
      assert(deferredFreeIndices[i].empty());
   }
#endif

   descriptorSize = device->GetDescriptorHandleIncrementSize(type);
   cpuStart = heap->GetCPUDescriptorHandleForHeapStart();
   gpuStart = isShaderVisible ? heap->GetGPUDescriptorHandleForHeapStart() : D3D12_GPU_DESCRIPTOR_HANDLE{ 0 };

   return true;
}


void d3d12DESCRIPTOR_HEAP::Release()
{
   assert(!size);
   d3d12DeferredRelease(heap);
}


void d3d12DESCRIPTOR_HEAP::ProcessDeferredFree(U32 frameIdx)
{
   std::lock_guard lock(mutex);
   assert(frameIdx < D3D12_FRAME_BUFFER_COUNT);

   dsVECTOR<U32>& indices = deferredFreeIndices[frameIdx];
   if (!indices.empty())
   {
      for (auto index : indices)
      {
         --size;
         freeHandles[size] = index;
      }

      indices.clear();
   }
}


d3d12DESCRIPTOR_HANDLE d3d12DESCRIPTOR_HEAP::Allocate()
{
   std::lock_guard lock(mutex);

   assert(heap);
   assert(size < capacity);

   const U32 index = freeHandles[size];
   const U32 offset = index * descriptorSize;
   ++size;

   d3d12DESCRIPTOR_HANDLE handle;
   handle.cpu.ptr = cpuStart.ptr + offset;
   if (IsShaderVisible())
   {
      handle.gpu.ptr = gpuStart.ptr + offset;
   }

   DEBUG_OP(handle.container = this);
   DEBUG_OP(handle.index = index);

   return handle;
}


void d3d12DESCRIPTOR_HEAP::Free(d3d12DESCRIPTOR_HANDLE& handle)
{
   if (!handle.IsValid())
   {
      return;
   }

   std::lock_guard lock(mutex);

   assert(heap && size);
   assert(handle.container == this);
   assert(handle.cpu.ptr >= cpuStart.ptr);
   assert((handle.cpu.ptr - cpuStart.ptr) % descriptorSize == 0);
   assert(handle.index < capacity);

   const U32 index = (U32)(handle.cpu.ptr - cpuStart.ptr) / descriptorSize;
   assert(handle.index == index);

   const U32 frameIndex = d3d12GetCurrentFrameIndex();
   deferredFreeIndices[frameIndex].push_back(index);
   d3d12SetDeferredReleasesFlag();

   handle = {};
}