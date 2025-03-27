#pragma once

#include <thread>

#define TEST_ENTITY_AND_COMPONENTS 0
#define TEST_WINDOW 0
#define TEST_RENDERER 1


class testTEST
{
public:
	virtual bool Initialize() = 0;
	virtual void Run() = 0;
	virtual void Shutdown() = 0;
};