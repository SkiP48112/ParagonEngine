#include "common.h"
#include "app_common_headers.h"

#ifndef WIN32_MEAN_AND_LEAN
	#define WIN32_MEAN_AND_LEAN
#endif // !WIN32_MEAN_AND_LEAN

#include <Windows.h>

namespace
{
	HMODULE gameCodeDll{ nullptr };
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

	return gameCodeDll ? TRUE : FALSE;
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