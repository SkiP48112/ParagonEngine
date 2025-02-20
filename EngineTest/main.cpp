#pragma comment(lib, "engine.lib")

#define TEST_ENTITY_AND_COMPONENTS 1;



#if TEST_ENTITY_AND_COMPONENTS
#include "test_entity_and_components.h"
#else
#error One of the test need to be enabled
#endif

int main()
{
#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	testENTITY_AND_COMPONENTS test{};
	if (test.Initialize())
	{
		test.Run();
	}

	test.Shutdown();
}