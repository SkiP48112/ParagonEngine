#ifndef SHIPPING

#include <thread>
#include "..\resources\res_content_loader.h"
#include "..\game_systems\gs_script.h"
#include "..\platform\app_platform_types.h"
#include "..\platform\app_platform.h"
#include "..\graphics\gr_renderer.h"


namespace
{
   grRENDER_SURFACE gameWindow;


   LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
   {
      switch (msg)
      {
      case WM_DESTROY:
      {
         if (gameWindow.window.IsClosed())
         {
            PostQuitMessage(0);
            return 0;
         }
      }
      break;
      case WM_SYSCHAR:
         if (wparam == VK_RETURN && (HIWORD(lparam) & KF_ALTDOWN))
         {
            gameWindow.window.SetFullscreen(!gameWindow.window.IsFullscreen());
            return 0;
         }
         break;
      }

      return DefWindowProc(hwnd, msg, wparam, lparam);
   }
}


bool coreInitialize()
{
	if (!resLoadGame())
	{
		return false;
	}

   appWINDOW_INIT_INFO info{ &WindowProc, nullptr, L"Paragin Game" };
   gameWindow.window = appCreateWindow(&info);
   if (!gameWindow.window.IsValid())
   {
      return false;
   }


	return true;
}


void coreUpdate()
{
	gsUpdateScripts(10.0f);
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
}


void coreShutdown()
{
   appRemoveWindow(gameWindow.window.GetID());
	resUnloadGame();
}

#endif