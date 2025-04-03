#pragma once

#include <thread>
#include <chrono>
#include <string>


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


#if _WIN64
#include <Windows.h>

class testTIMER
{
public:
	using CLOCK = std::chrono::high_resolution_clock;
	using TIME_STAMP = std::chrono::steady_clock::time_point;

	void Begin()
	{
		start = CLOCK::now();
	}

	void End()
	{
		auto dt = CLOCK::now() - start;
		msAvg += ((float)std::chrono::duration_cast<std::chrono::milliseconds>(dt).count() - msAvg) / (float)counter;
		++counter;

		if (std::chrono::duration_cast<std::chrono::seconds>(CLOCK::now() - seconds).count() >= 1)
		{
			OutputDebugStringA("Avg. frame (ms): ");
			OutputDebugStringA(std::to_string(msAvg).c_str());
			OutputDebugStringA((" " + std::to_string(counter)).c_str());
			OutputDebugStringA(" fps");
			OutputDebugStringA("\n");

			msAvg = 0.0f;
			counter = 1;
			seconds = CLOCK::now();
		}
	}

private:
	float msAvg = 0.0f;
	int counter = 1;
	TIME_STAMP start;
	TIME_STAMP seconds = CLOCK::now();
};

#endif