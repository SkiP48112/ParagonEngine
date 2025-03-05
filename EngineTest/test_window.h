#pragma once

#include "test.h"
#include "..\platform\app_platform_types.h"
#include "..\platform\app_platform.h"


appWINDOW windows[4];


LRESULT testWinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
   switch (msg)
   {
   case WM_DESTROY:
   {
      bool isAllWindowsClosed = true;
      for (U32 i = 0; i < _countof(windows); ++i)
      {
         if (!windows[i].IsClosed())
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


class testENGINE_TEST : public testTEST
{
public:
   bool Initialize() override
   {
      appWINDOW_INIT_INFO info[]{
         {&testWinProc, nullptr, L"Test window 1", 100, 100, 400, 800},
         {&testWinProc, nullptr, L"Test window 2", 150, 150, 800, 400},
         {&testWinProc, nullptr, L"Test window 3", 200, 200, 400, 400},
         {&testWinProc, nullptr, L"Test window 4", 250, 250, 800, 800},
      };
      static_assert(_countof(info) == _countof(windows));

      for (U32 i = 0; i < _countof(windows); ++i)
      {
         windows[i] = appCreateWindow(&info[i]);
      }

      return true;
   }


   void Run() override
   {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
   }


   void Shutdown() override
   {
      for (U32 i = 0; i < _countof(windows); ++i)
      {
         appRemoveWindow(windows[i].GetID());
      }
   }
};