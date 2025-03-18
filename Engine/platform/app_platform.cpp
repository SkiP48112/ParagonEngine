#include "app_platform.h"
#include "app_platform_types.h"


#ifdef  _WIN64

namespace
{
   struct appWINDOW_DESC
   {
      HWND hwnd = nullptr;
      RECT clientArea{ 0, 0, 1920, 1080 };
      RECT fullscreenArea;
      POINT topLeft{ 0, 0 };
      DWORD style = WS_VISIBLE;

      bool isFullscreen = false;
      bool isClosed = false;
   };


   dsVECTOR<appWINDOW_DESC> windows;
   dsVECTOR<U32> availableSlots;


   // TODO: This method should be replaced when a free-list container would be implemented
   U32 appAddToWindows(appWINDOW_DESC& desc)
   {
      U32 id = U32_INVALID_ID;

      if (availableSlots.empty())
      {
         id = (U32)windows.size();
         windows.emplace_back(desc);
      } 
      else
      {
         id = availableSlots.back();
         availableSlots.pop_back();
         assert(id != U32_INVALID_ID);
         windows[id] = desc;
      }

      return id;
   }


   void appRemoveFromWindows(U32 id)
   {
      assert(id < windows.size());
      availableSlots.emplace_back(id);
   }


   appWINDOW_DESC& appGetWindowDescById(appWINDOW_ID id)
   {
      assert(id < windows.size());
      assert(windows[id].hwnd);

      return windows[id];
   }


   appWINDOW_DESC& appGetWindowDescByHandle(appWINDOW_HANDLE handle)
   {
      const appWINDOW_ID id{ (idID_TYPE)GetWindowLongPtr(handle, GWLP_USERDATA) };
      return appGetWindowDescById(id);
   }


   LRESULT CALLBACK appGetInternalWindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
   {
      appWINDOW_DESC* desc = nullptr;
      switch (msg)
      {
      case WM_DESTROY:
         appGetWindowDescByHandle(hwnd).isClosed = true;
         break;
      case WM_EXITSIZEMOVE:
         desc = &appGetWindowDescByHandle(hwnd);
         break;
      case WM_SIZE:
         if (wparam == SIZE_MAXIMIZED)
         {
            desc = &appGetWindowDescByHandle(hwnd);
         }
         break;
      case WM_SYSCOMMAND:
         if (wparam == SC_RESTORE)
         {
            desc = &appGetWindowDescByHandle(hwnd);
         }
         break;
      default:
         break;
      }

      if (desc)
      {
         assert(desc->hwnd);
         GetClientRect(desc->hwnd, desc->isFullscreen ? &desc->fullscreenArea : &desc->clientArea);
      }

      LONG_PTR pLong = GetWindowLongPtr(hwnd, 0);
      if (pLong)
      {
         return ((appWINDOW_PROC)pLong)(hwnd, msg, wparam, lparam);
      }

      return DefWindowProc(hwnd, msg, wparam, lparam);
   }


   WNDCLASSEX appSetupWindowClass(appWINDOW_PROC& callback)
   {
      WNDCLASSEX wc;
      ZeroMemory(&wc, sizeof(wc));

      wc.cbSize = sizeof(WNDCLASSEX);
      wc.style = CS_HREDRAW | CS_VREDRAW;

      wc.lpfnWndProc = appGetInternalWindowProc;
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


   appWINDOW_DESC appCreateWindowInstance(const appWINDOW_INIT_INFO* const initInfo, WNDCLASSEX& wc, appWINDOW_HANDLE& parent)
   {
      appWINDOW_DESC desc;

      desc.clientArea.right = (initInfo && initInfo->width) ? desc.clientArea.left + initInfo->width : desc.clientArea.right;
      desc.clientArea.bottom = (initInfo && initInfo->height) ? desc.clientArea.top + initInfo->height : desc.clientArea.bottom;
      desc.style |= parent ? WS_CHILD : WS_OVERLAPPEDWINDOW;

      RECT rect(desc.clientArea);
      AdjustWindowRect(&rect, desc.style, FALSE);

      const wchar_t* caption = (initInfo && initInfo->caption) ? initInfo->caption : L"Paragon Game";
      const S32 left = initInfo? initInfo->left : desc.topLeft.x;
      const S32 top = initInfo ? initInfo->top : desc.topLeft.y;
      const S32 width = rect.right - rect.left;
      const S32 height = rect.bottom - rect.top;

      desc.hwnd = CreateWindowEx(
         0,                // extended style
         wc.lpszClassName, // window class name
         caption,          // instance title
         desc.style,       // window style
         left,             // initial window position
         top,
         width,            // initial window dimensions
         height,
         parent,           // handle to parent window
         NULL,             // handle to menu
         NULL,             // instance of this application
         NULL              // extra creation params
      );

      return desc;
   }


   const mVECTOR4_U32 appGetWindowSize(const appWINDOW_ID id)
   {
      appWINDOW_DESC& desc = appGetWindowDescById(id);
      RECT& area = desc.isFullscreen ? desc.fullscreenArea : desc.clientArea;

      return { (U32)area.left, (U32)area.top, (U32)area.right, (U32)area.bottom };
   }


   appWINDOW_HANDLE appGetWindowHandle(const appWINDOW_ID id)
   {
      return appGetWindowDescById(id).hwnd;
   }


   bool appIsWindowFullscreen(const appWINDOW_ID id)
   {
      return appGetWindowDescById(id).isFullscreen;
   }


   void appSetWindowCaption(appWINDOW_ID id, const wchar_t* caption)
   {
      appWINDOW_DESC& desc = appGetWindowDescById(id);
      SetWindowText(desc.hwnd, caption);
   }


   void appResizeWindow(const appWINDOW_DESC& desc, const RECT& area)
   {
      RECT windowRect = area;
      AdjustWindowRect(&windowRect, desc.style, FALSE);

      const S32 width = windowRect.right - windowRect.left;
      const S32 height = windowRect.bottom - windowRect.top;

      MoveWindow(desc.hwnd, desc.topLeft.x, desc.topLeft.y, width, height, true);
   }


   void appResizeWindow(const appWINDOW_ID id, U32 width, U32 height)
   {
      appWINDOW_DESC& desc = appGetWindowDescById(id);

      if (desc.style & WS_CHILD)
      {
         GetClientRect(desc.hwnd, &desc.clientArea);
         return;
      }

      // NOTE: we also resize while in fullscreen mode to support the case
      //       when the user changes the screen resolution

      RECT& area = desc.isFullscreen ? desc.fullscreenArea : desc.clientArea;
      area.bottom = area.top + height;
      area.bottom = area.top + height;

      appResizeWindow(desc, area);
   }


   void appSetWindowFullscreen(appWINDOW_DESC& desc)
   {
      RECT rect;
      GetClientRect(desc.hwnd, &desc.clientArea);
      GetWindowRect(desc.hwnd, &rect);

      desc.topLeft.x = rect.left;
      desc.topLeft.y = rect.top;

      SetWindowLongPtr(desc.hwnd, GWL_STYLE, NULL);
      ShowWindow(desc.hwnd, SW_MAXIMIZE);
   }


   void appSetWindowNormalScreen(appWINDOW_DESC& desc)
   {
      SetWindowLongPtr(desc.hwnd, GWL_STYLE, desc.style);
      appResizeWindow(desc, desc.clientArea);
      ShowWindow(desc.hwnd, SW_SHOWNORMAL);
   }


   void appSetWindowScreenType(appWINDOW_ID id, bool isFullscreen)
   {
      appWINDOW_DESC& desc = appGetWindowDescById(id);
      if (desc.isFullscreen == isFullscreen)
      {
         return;
      }

      desc.isFullscreen = isFullscreen;
      if (isFullscreen)
      {
         appSetWindowFullscreen(desc);
         return;
      }

      appSetWindowNormalScreen(desc);
   }


   bool appIsWindowClosed(appWINDOW_ID id)
   {
      return appGetWindowDescById(id).isClosed;
   }
}


appWINDOW appCreateWindow(const appWINDOW_INIT_INFO* const initInfo)
{
   appWINDOW_PROC callback = initInfo ? initInfo->callback : nullptr;
   appWINDOW_HANDLE parent = initInfo ? initInfo->parent : nullptr;

   WNDCLASSEX wc = appSetupWindowClass(callback);
   RegisterClassEx(&wc);
   appWINDOW_DESC desc = appCreateWindowInstance(initInfo, wc, parent);

   if (!desc.hwnd)
   {
      return appWINDOW();
   }

   // NOTE: RegisterClassEx() throws an error if we try to register same calss twice
   //       So we reset last error, to fix this problem
   // TODO: Find a more proper way to fix this problem
   DEBUG_OP(SetLastError(0));

   appWINDOW_ID id = (appWINDOW_ID)appAddToWindows(desc);
   SetWindowLongPtr(desc.hwnd, GWLP_USERDATA, (LONG_PTR)id);

   if (callback != nullptr)
   {
      SetWindowLongPtr(desc.hwnd, 0, (LONG_PTR)callback);
   }

   assert(GetLastError() == 0);
   ShowWindow(desc.hwnd, SW_SHOWNORMAL);
   UpdateWindow(desc.hwnd);
   return appWINDOW(id);
}


void appRemoveWindow(appWINDOW_ID id)
{
   appWINDOW_DESC& info = appGetWindowDescById(id);
   
   DestroyWindow(info.hwnd);
   appRemoveFromWindows(id);
}


#else
#error "used platform is not implented yet"
#endif


mVECTOR4_U32 appWINDOW::GetSize() const
{
   assert(IsValid());
   return appGetWindowSize(id);
}


U32 appWINDOW::GetWidth() const
{
   assert(IsValid());
   mVECTOR4_U32 size = GetSize();

   return size.z - size.x;
}


U32 appWINDOW::GetHeight() const
{
   assert(IsValid());
   mVECTOR4_U32 size = GetSize();

   return size.w - size.y;
}


void* appWINDOW::GetHandle() const
{
   assert(IsValid());
   return appGetWindowHandle(id);
}


bool appWINDOW::IsFullscreen() const
{
   assert(IsValid());
   return appIsWindowFullscreen(id);
}


bool appWINDOW::IsClosed() const
{
   assert(IsValid());
   return appIsWindowClosed(id);
}


void appWINDOW::SetCaption(const wchar_t* caption) const
{
   assert(IsValid());
   appSetWindowCaption(id, caption);
}


void appWINDOW::SetFullscreen(bool isFullscreen) const
{
   assert(IsValid());
   appSetWindowScreenType(id, isFullscreen);
}


void appWINDOW::Resize(U32 width, U32 height) const
{
   assert(IsValid());
   appResizeWindow(id, width, height);
}