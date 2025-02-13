#pragma once

#include <iostream>
#include <ctime>

#include "test.h"
#include "..\Engine\components\entity.h"
#include "..\Engine\components\transform.h"

using namespace paragon;

class TestEntityAndComponents : public Test
{
public:
	bool Initialize() override
	{
		srand((U32)time(nullptr));
		return true; 
	};

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
	{ }


private:
	void CreateRandom()
	{
		U32 count = rand() % 20;
		if (entities.empty())
		{
			count = 1000;
		}

		transform::InitInfo transformInfo{};
		game_entity::EntityInfo entityInfo{ &transformInfo };
		while (count > 0)
		{
			game_entity::Entity entity{ game_entity::CreateGameEntity(entityInfo) };
			assert(entity.IsValid() && idIsValid(entity.GetID()));

			entities.push_back(entity);
			assert(game_entity::IsAlive(entity));

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
			const U32 index{(U32)rand() % (U32)entities.size()};
			const game_entity::Entity entity{ entities[index] };
			assert(entity.IsValid() && idIsValid(entity.GetID()));
			if (entity.IsValid())
			{
				game_entity::RemoveGameEntity(entity);
				entities.erase(entities.begin() + index);
				assert(!game_entity::IsAlive(entity));
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

	ds::Vector<game_entity::Entity> entities;

	U32 added{ 0 };
	U32 removed{ 0 };
	U32 amount{ 0 };

};