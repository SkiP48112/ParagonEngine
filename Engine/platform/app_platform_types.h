#pragma once

#include "app_common_headers.h"


#ifdef _WIN64

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>


using appWINDOW_PROC = LRESULT(*)(HWND, UINT, WPARAM, LPARAM);
using appWINDOW_HANDLE = HWND;


struct appWINDOW_INIT_INFO
{
   appWINDOW_PROC callback = nullptr;
   appWINDOW_HANDLE parent = nullptr;
   const wchar_t* caption = nullptr;

   S32 left = 0;
   S32 top = 0;

   S32 width = 1920;
   S32 height = 1080;
};


#endif
