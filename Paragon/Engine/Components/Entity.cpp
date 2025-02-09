#include "entity.h"
#include "transform.h"

namespace paragon::game_entity
{
	namespace
	{
		utl::Vector<transform::Component> transforms;
		utl::Vector<id::GenerationType> generations;
		utl::Deque<EntityID> freeIds;
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
		if (freeIds.size() > id::MIN_DELETED_ELEMENTS)
		{
			id = freeIds.front();
			assert(!IsAlive(Entity{ id }));

			freeIds.pop_front();
			id = EntityID{ id::NewGeneration(id) };
			++generations[id::Index(id)];
		}
		else
		{
			id = EntityID{(id::IDType)generations.size()};
			generations.push_back(0);

			// Resize components
			// NOTE: Don't want to use resize(), so the number of memory allocations stays low
			transforms.emplace_back();
		}

		const Entity newEntity{ id };
		const id::IDType index{ id::Index(id) };

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
		const id::IDType index{ id::Index(id) };

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
		const id::IDType index{ id::Index(id) };

		assert(index < generations.size());
		assert(generations[index] == id::Generation(id));
	
		return (generations[index] == id::Generation(id) && transforms[index].IsValid());
	}

	transform::Component Entity::Transform() const
	{
		assert(IsAlive(*this));

		const id::IDType index{  id::Index(_id) };
		return transforms[index];
	}
}