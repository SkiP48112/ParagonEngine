#if _WIN64

#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif 

#include <Windows.h>
#include <crtdbg.h>
#ifndef USE_WITH_EDITOR

extern bool coreInitialize();
extern void coreUpdate();
extern void coreShutdown();

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	if (coreInitialize())
	{
		MSG msg{};
		bool isRunning = true;
		while (isRunning)
		{
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				isRunning &= (msg.message != WM_QUIT);
			}

			coreUpdate();
		}
	}

	coreShutdown();
	return 0;
}

#endif


#endif