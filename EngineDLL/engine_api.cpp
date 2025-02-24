#include "common.h"
#include "app_common_headers.h"
#include "..\Engine\engine_api\api_game_entity.h"


#ifndef WIN32_
	#define WIN32_MEAN_AND_LEAN
#endif // !WIN32_MEAN_AND_LEAN

#include <Windows.h>

namespace
{
	HMODULE gameCodeDll{ nullptr };

	using geSCRIPT_CREATOR_PTR = geSCRIPT_CREATOR(*)(size_t);
	geSCRIPT_CREATOR_PTR apiGetScriptCreatorPtr{ nullptr };

	using LPSAFEARRAY_PTR = LPSAFEARRAY(*)(void);
	LPSAFEARRAY_PTR apiGetScriptNamesPtr{ nullptr };
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
	apiGetScriptCreatorPtr = (geSCRIPT_CREATOR_PTR)GetProcAddress(gameCodeDll, "apiGetScriptCreator");

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
geSCRIPT_CREATOR GetScriptCreator(const char* name)
{
	return (gameCodeDll && apiGetScriptCreatorPtr) ? apiGetScriptCreatorPtr(dsSTRING_HASH()(name)) : nullptr;
}


EDITOR_INTERFACE
LPSAFEARRAY GetScriptNames()
{
	return (gameCodeDll && apiGetScriptNamesPtr) ? apiGetScriptNamesPtr() : nullptr;
}