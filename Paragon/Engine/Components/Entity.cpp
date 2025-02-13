#include "entity.h"
#include "transform.h"

namespace paragon::game_entity
{
	namespace
	{
		ds::Vector<transform::Component> transforms;
		ds::Vector<idGENERATION_TYPE> generations;
		ds::Deque<EntityID> freeIds;
	}


	Entity CreateGameEntity(const EntityInfo info)
	{
		// Every game entity must have a transform component
		assert(info.transform); 
		if (!info.transform)
		{
			return Entity{};
		}

		EntityID id;
		if (freeIds.size() > idMIN_DELETED_ELEMENTS)
		{
			id = freeIds.front();
			assert(!IsAlive(Entity{ id }));

			freeIds.pop_front();
			id = EntityID{ idNewGeneration(id) };
			++generations[idGetIndex(id)];
		}
		else
		{
			id = EntityID{(idID_TYPE)generations.size()};
			generations.push_back(0);

			// Resize components
			// NOTE: Don't want to use resize(), so the number of memory allocations stays low
			transforms.emplace_back();
		}

		const Entity newEntity{ id };
		const idID_TYPE index{ idGetIndex(id) };

		// Create transform component
		assert(!transforms[index].IsValid());
		transforms[index] = transform::CreateTransform(*info.transform, newEntity);
		if (!transforms[index].IsValid())
		{
			return {};
		}

		return newEntity;
	}


	void RemoveGameEntity(Entity entity)
	{
		const EntityID id{ entity.GetID() };
		const idID_TYPE index{ idGetIndex(id) };

		assert(IsAlive(entity));
		if (IsAlive(entity))
		{
			transform::RemoveTrasnform(transforms[index]);
			transforms[index] = {};

			freeIds.push_back(id);
		}
	}


	bool IsAlive(Entity entity)
	{
		assert(entity.IsValid());
		
		const EntityID id{ entity.GetID() };
		const idID_TYPE index{ idGetIndex(id) };

		assert(index < generations.size());
		assert(generations[index] == idGetGeneration(id));
	
		return (generations[index] == idGetGeneration(id) && transforms[index].IsValid());
	}


	transform::Component Entity::Transform() const
	{
		assert(IsAlive(*this));

		const idID_TYPE index{  idGetIndex(id) };
		return transforms[index];
	}
}