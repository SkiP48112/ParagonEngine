#pragma once

#include <dxgi1_6.h>
#include <d3d12.h>
#include <wrl.h>

#include "app_common_headers.h"
#include "graphics/gr_renderer.h"
#include "platform/app_window.h"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")


constexpr U32 D3D12_FRAME_BUFFER_COUNT = 3;


// Assert that COM call to D3D API succeded

#ifdef _DEBUG
   #ifndef ASSERT_DX
      #define ASSERT_DX(x)                         \
      if(FAILED(x)) {                              \
         char lineNumber[32];                      \
         sprintf_s(lineNumber, "%u", __LINE__);    \
         OutputDebugStringA("Error in: ");         \
         OutputDebugStringA(__FILE__);             \
         OutputDebugStringA("\nLine: ");           \
         OutputDebugStringA(lineNumber);           \
         OutputDebugStringA("\n");                 \
         OutputDebugStringA(#x);                   \
         OutputDebugStringA("\n");                 \
         __debugbreak();                           \
      }
   #endif
#else
   #ifndef ASSERT_DX
      #define ASSERT_DX(x) x
   #endif 
#endif


#ifdef _DEBUG
   #define NAME_D3D12_OBJECT(obj, name) obj->SetName(name); OutputDebugString(L"::D3D12 Object Created: "); OutputDebugString(name); OutputDebugString(L"\n");
   #define NAME_D3D12_INDEXED_OBJECT(obj, index, name)            \
   {                                                              \
      wchar_t fullName[128];                                      \
      if (swprintf_s(fullName, L"%s[%u]", name, index) > 0)       \
      {                                                           \
         obj->SetName(fullName);                                  \
         OutputDebugString(L"::D3D12 Object Created: ");          \
         OutputDebugString(fullName);                             \
         OutputDebugString(L"\n");                                \
      }                                                           \
   }
#else
   #define NAME_D3D12_OBJECT(obj, name)
   #define NAME_D3D12_INDEXED_OBJECT(obj, index, name)
#endif