#pragma once

#include "d3d12_common_headers.h"

class d3d12DESCRIPTOR_HEAP;


struct d3d12DESCRIPTOR_HANDLE
{

public:
   constexpr bool IsValid() const
   {
      return cpu.ptr != 0;
   }


   constexpr bool IsShaderVisible() const
   {
      return gpu.ptr != 0;
   }

public:
   D3D12_CPU_DESCRIPTOR_HANDLE cpu{};
   D3D12_GPU_DESCRIPTOR_HANDLE gpu{};

private:
#ifdef _DEBUG
   friend class d3d12DESCRIPTOR_HEAP;
   
   d3d12DESCRIPTOR_HEAP* container = nullptr;
   U32 index = U32_INVALID_ID;
#endif
};


class d3d12DESCRIPTOR_HEAP
{
public:
   DISABLE_COPY_AND_MOVE(d3d12DESCRIPTOR_HEAP);

   
   explicit d3d12DESCRIPTOR_HEAP(D3D12_DESCRIPTOR_HEAP_TYPE type) : type{type}
   {
   }


   ~d3d12DESCRIPTOR_HEAP()
   {
      assert(!heap);
   }


   constexpr D3D12_DESCRIPTOR_HEAP_TYPE GetType() const
   {
      return type;
   }

   constexpr D3D12_CPU_DESCRIPTOR_HANDLE GetCpuStart() const
   {
      return cpuStart;
   }

   constexpr D3D12_GPU_DESCRIPTOR_HANDLE GetGpuStart() const
   {
      return gpuStart;
   }

   constexpr ID3D12DescriptorHeap* const GetHeap() const
   {
      return heap;
   }

   constexpr U32 GetCapacity() const
   {
      return capacity;
   }

   constexpr U32 GetSize() const
   {
      return size;
   }

   constexpr U32 GetDescriptorSize() const
   {
      return descriptorSize;
   }

   constexpr bool IsShaderVisible() const
   {
      return gpuStart.ptr != 0;
   }


   bool Initialize(U32 capacity, bool isShaderVisible);
   void Release();
   void ProcessDeferredFree(U32 frameIdx);

   [[nodiscard]] d3d12DESCRIPTOR_HANDLE Allocate();
   void Free(d3d12DESCRIPTOR_HANDLE& handle);

private:
   const D3D12_DESCRIPTOR_HEAP_TYPE type;

   ID3D12DescriptorHeap* heap = nullptr;
   D3D12_CPU_DESCRIPTOR_HANDLE cpuStart{};
   D3D12_GPU_DESCRIPTOR_HANDLE gpuStart{};

   dsVECTOR<U32> deferredFreeIndices[D3D12_FRAME_BUFFER_COUNT];

   std::unique_ptr<U32[]> freeHandles;
   std::mutex mutex;

   U32 descriptorSize = 0;
   U32 capacity = 0;
   U32 size = 0;

};