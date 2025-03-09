#include "common.h"
#include "app_common_headers.h"
#include "..\Engine\engine_api\api_game_entity.h"
#include "..\platform\app_platform_types.h"
#include "..\platform\app_platform.h"
#include "..\graphics\gr_renderer.h"


#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <atlsafe.h>

namespace
{
	HMODULE gameCodeDll{ nullptr };

	using gsSCRIPT_CREATOR_PTR = gsSCRIPT_CREATOR(*)(size_t);
	gsSCRIPT_CREATOR_PTR apiGetScriptCreatorPtr{ nullptr };

	using LPSAFEARRAY_PTR = LPSAFEARRAY(*)(void);
	LPSAFEARRAY_PTR apiGetScriptNamesPtr{ nullptr };

	dsVECTOR<grRENDER_SURFACE> surfaces;
}

EDITOR_INTERFACE
U32 LoadGameCodeDll(const char* dllPath)
{
	if (gameCodeDll)
	{
		return FALSE;
	}

	gameCodeDll = LoadLibraryA(dllPath);
	assert(gameCodeDll);

	apiGetScriptNamesPtr = (LPSAFEARRAY_PTR)GetProcAddress(gameCodeDll, "apiGetScriptNames");
	apiGetScriptCreatorPtr = (gsSCRIPT_CREATOR_PTR)GetProcAddress(gameCodeDll, "apiGetScriptCreator");

	return (gameCodeDll && apiGetScriptNamesPtr && apiGetScriptCreatorPtr) ? TRUE : FALSE;
}

EDITOR_INTERFACE
U32 UnloadGameCodeDll()
{
	if (!gameCodeDll)
	{
		return FALSE;
	}

	assert(gameCodeDll);
	int result{FreeLibrary(gameCodeDll) };
	assert(result);

	gameCodeDll = nullptr;
	return TRUE;
}


EDITOR_INTERFACE
gsSCRIPT_CREATOR GetScriptCreator(const char* name)
{
	return (gameCodeDll && apiGetScriptCreatorPtr) ? apiGetScriptCreatorPtr(dsSTRING_HASH()(name)) : nullptr;
}


EDITOR_INTERFACE
LPSAFEARRAY GetScriptNames()
{
	return (gameCodeDll && apiGetScriptNamesPtr) ? apiGetScriptNamesPtr() : nullptr;
}


EDITOR_INTERFACE
U32 CreateRenderSurface(HWND host, S32 width, S32 height)
{
	assert(host);
	appWINDOW_INIT_INFO info{ nullptr, host, nullptr, 0, 0, width, height };
	
	grRENDER_SURFACE surface{ appCreateWindow(&info), {} };
	assert(surface.window.IsValid());

	U32 surfaceIdx = surfaces.size();
	surfaces.emplace_back(surface);
	return surfaceIdx;
}


EDITOR_INTERFACE
void RemoveRenderSurface(U32 idx)
{
	assert(idx < surfaces.size());
	appRemoveWindow(surfaces[idx].window.GetID());

	// TODO: Remove window from surfaces list when we have a free-list container
}


EDITOR_INTERFACE
HWND GetWindowHandle(U32 idx)
{
	assert(idx < surfaces.size());
	return (HWND)surfaces[idx].window.GetHandle();
}


EDITOR_INTERFACE
void ResizeRenderSurface(U32 idx)
{
	assert(idx < surfaces.size());
	surfaces[idx].window.Resize();
}