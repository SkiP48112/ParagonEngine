#include "app_platform.h"
#include "app_platform_types.h"


#ifdef  _WIN64

namespace
{
   struct WINDOW_INFO
   {
      HWND hwnd = nullptr;
      RECT clientArea{ 0, 0, 1920, 1080 };
      RECT fullscreenArea;
      POINT topLeft{ 0, 0 };
      DWORD style = WS_VISIBLE;

      bool isFullscreen = false;
      bool isClosed = false;
   };


   LRESULT CALLBACK GetInternalWindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
   {
      LONG_PTR pLong = GetWindowLongPtr(hwnd, 0);
      if (pLong)
      {
         return ((appWINDOW_PROC)pLong)(hwnd, msg, wparam, lparam);
      }

      return DefWindowProc(hwnd, msg, wparam, lparam);
   }


   WNDCLASSEX SetupWindowClass(appWINDOW_PROC& callback)
   {
      WNDCLASSEX wc;
      ZeroMemory(&wc, sizeof(wc));

      wc.cbSize = sizeof(WNDCLASSEX);
      wc.style = CS_HREDRAW | CS_VREDRAW;

      wc.lpfnWndProc = GetInternalWindowProc;
      wc.cbClsExtra = 0;
      wc.cbWndExtra = callback ? sizeof(callback) : 0;
      
      wc.hInstance = 0;
      wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
      wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
      wc.hCursor = LoadCursor(NULL, IDC_ARROW);
      wc.hbrBackground = CreateSolidBrush(RGB(26, 48, 76));

      wc.lpszMenuName = NULL;
      wc.lpszClassName = L"ParagonWindow";

      return wc;
   }


   WINDOW_INFO CreateWindowInstance(const appWINDOW_INIT_INFO* const initInfo, WNDCLASSEX& wc, appWINDOW_HANDLE& parent)
   {
      WINDOW_INFO info;
      RECT rc(info.clientArea);

      AdjustWindowRect(&rc, info.style, FALSE);

      const wchar_t* caption = (initInfo && initInfo->caption) ? initInfo->caption : L"Paragon Game";
      const S32 left = (initInfo && initInfo->left) ? initInfo->left : info.clientArea.left;
      const S32 top = (initInfo && initInfo->top) ? initInfo->top : info.clientArea.top;
      const S32 width = (initInfo && initInfo->width) ? initInfo->width : rc.right - rc.left;
      const S32 height = (initInfo && initInfo->height) ? initInfo->height : rc.bottom - rc.top;

      info.style |= parent ? WS_CHILD : WS_OVERLAPPEDWINDOW;

      info.hwnd = CreateWindowEx(
         0,                // extended style
         wc.lpszClassName, // window class name
         caption,          // instance title
         info.style,       // window style
         left,             // initial window position
         top,
         width,            // initial window dimensions
         height,
         parent,           // handle to parent window
         NULL,             // handle to menu
         NULL,             // instance of this application
         NULL              // extra creation params
      );

      return info;
   }
}


appWINDOW appCreateWindow(const appWINDOW_INIT_INFO* const initInfo)
{
   appWINDOW_PROC callback = initInfo ? initInfo->callback : nullptr;
   appWINDOW_HANDLE parent = initInfo ? initInfo->parent : nullptr;

   WNDCLASSEX wc = SetupWindowClass(callback);
   RegisterClassEx(&wc);
   WINDOW_INFO info = CreateWindowInstance(initInfo, wc, parent);



   return appWINDOW();
}

#elif
#error "used platform is not implented yet"
#endif