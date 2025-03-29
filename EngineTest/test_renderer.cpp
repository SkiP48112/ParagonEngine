#include "test_renderer.h"
#include "..\platform\app_platform_types.h"
#include "..\platform\app_platform.h"
#include "..\graphics\gr_renderer.h"

#if TEST_RENDERER

grRENDER_SURFACE surfaces[4];


LRESULT testWinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
   switch (msg)
   {
   case WM_DESTROY:
   {
      bool isAllWindowsClosed = true;
      for (U32 i = 0; i < _countof(surfaces); ++i)
      {
         if (!surfaces[i].window.IsClosed())
         {
            isAllWindowsClosed = false;
            break;
         }
      }

      if (isAllWindowsClosed)
      {
         PostQuitMessage(0);
         return 0;
      }
   }
   break;
   case WM_SYSCHAR:
      if (wparam == VK_RETURN && (HIWORD(lparam) & KF_ALTDOWN))
      {
         appWINDOW win(appWINDOW_ID(GetWindowLongPtr(hwnd, GWLP_USERDATA)));
         win.SetFullscreen(!win.IsFullscreen());
         return 0;
      }
      break;
   }

   return DefWindowProc(hwnd, msg, wparam, lparam);
}


void testCreateRenderSurface(grRENDER_SURFACE& surface, appWINDOW_INIT_INFO info)
{
   surface.window = appCreateWindow(&info);
}


void testDestroyRenderSurface(grRENDER_SURFACE& surface)
{
   appRemoveWindow(surface.window.GetID());
}


bool testENGINE_TEST::Initialize()
{
   bool result = grInitialize(grGRAPHICS_PLATFORM::GP_DIRECT3D_12);
   if (!result)
   {
      return result;
   }

   appWINDOW_INIT_INFO info[]{
         {&testWinProc, nullptr, L"Test window 1", 100, 100, 400, 800},
         {&testWinProc, nullptr, L"Test window 2", 150, 150, 800, 400},
         {&testWinProc, nullptr, L"Test window 3", 200, 200, 400, 400},
         {&testWinProc, nullptr, L"Test window 4", 250, 250, 800, 800},
   };
   static_assert(_countof(info) == _countof(surfaces));

   for (U32 i = 0; i < _countof(surfaces); ++i)
   {
      testCreateRenderSurface(surfaces[i], info[i]);
   }

   return result;
}


void testENGINE_TEST::Run()
{
   std::this_thread::sleep_for(std::chrono::milliseconds(10));
   grRender();
}


void testENGINE_TEST::Shutdown()
{
   for (U32 i = 0; i < _countof(surfaces); ++i)
   {
      testDestroyRenderSurface(surfaces[i]);
   }

   grShutdown();
}

#endif