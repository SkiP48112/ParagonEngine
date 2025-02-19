#pragma once

#include <iostream>
#include <ctime>
#include "test.h"
#include "..\Engine\game_entities\ge_entity.h"
#include "..\Engine\game_entities\ge_transform.h"


class testENTITY_AND_COMPONENTS : public testTEST
{
public:
	bool Initialize() override
	{
		srand((U32)time(nullptr));
		return true;
	}

	void Run() override
	{
		do
		{
			for (U32 i = 0; i < 10000; ++i)
			{
				CreateRandom();
				RemoveRandom();
				amount = (U32)entities.size();
			}

			PrintResults();
		} while (getchar() != 'q');
	}

	void Shutdown() override
	{
	}


private:
	void CreateRandom()
	{
		U32 count = rand() % 20;
		if (entities.empty())
		{
			count = 1000;
		}

		geTRANSFORM_INIT_INFO transformInfo{};
		geENTITY_INFO entityInfo{ &transformInfo };
		while (count > 0)
		{
			geENTITY entity{ geCreateGameEntity(entityInfo) };
			assert(entity.IsValid() && idIsValid(entity.GetID()));

			entities.push_back(entity);
			assert(geIsAlive(entity.GetID()));

			added++;
			count--;
		}
	}

	void RemoveRandom()
	{
		U32 count = rand() % 20;
		if (entities.size() < 1000)
		{
			return;
		}

		while (count > 0)
		{
			const U32 index{ (U32)rand() % (U32)entities.size() };
			const geENTITY entity{ entities[index] };
			assert(entity.IsValid() && idIsValid(entity.GetID()));
			if (entity.IsValid())
			{
				geRemoveGameEntity(entity.GetID());
				entities.erase(entities.begin() + index);
				assert(!geIsAlive(entity.GetID()));
			}

			removed++;
			count--;
		}
	}

	void PrintResults()
	{
		std::cout << "Entities created " << added << std::endl;
		std::cout << "Entities removed " << removed << std::endl;
	}

private:
	dsVECTOR<geENTITY> entities;

	U32 added{ 0 };
	U32 removed{ 0 };
	U32 amount{ 0 };
};