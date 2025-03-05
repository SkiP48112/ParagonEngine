#pragma once

#include "app_common_headers.h"
#include "app_window.h"

struct appWINDOW_INIT_INFO;

appWINDOW appCreateWindow(const appWINDOW_INIT_INFO* const initInfo = nullptr);
void appRemoveWindow(appWINDOW_ID id);