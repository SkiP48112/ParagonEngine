#pragma comment(lib, "engine.lib")

#define TEST_ENTITY_AND_COMPONENTS 0;
#define TEST_WINDOW 0;
#define TEST_RENDERER 1;



#if TEST_ENTITY_AND_COMPONENTS
	#include "test_entity_and_components.h"
#elif TEST_WINDOW
	#include "test_window.h"
#elif TEST_RENDERER
	#include "test_renderer.h"
#else
	#error One of the test need to be enabled
#endif


#ifdef _WIN64
#include <windows.h>


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	testENGINE_TEST test;
	if (test.Initialize())
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

			test.Run();
		}
	}

	test.Shutdown();
	return 0;
}

#else

int main()
{
#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	testENGINE_TEST test;
	if (test.Initialize())
	{
		test.Run();
	}

	test.Shutdown();
}

#endif