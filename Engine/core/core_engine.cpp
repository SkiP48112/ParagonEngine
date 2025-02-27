#ifndef SHIPPING

#include <thread>
#include "..\resources\res_content_loader.h"
#include "..\game_entities\ge_script.h"


bool coreInitialize()
{
	return resLoadGame();
}


void coreUpdate()
{
	geUpdateScripts(10.0f);
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
}


void coreShutdown()
{
	resUnloadGame();
}

#endif