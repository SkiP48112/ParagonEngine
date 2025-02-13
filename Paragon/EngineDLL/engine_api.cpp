#include "common_headers.h"
#include "..\Engine\components\entity.h"
#include "..\Engine\components\transform.h"
#include "id.h"

#ifndef EDITOR_INTERFACE
	#define EDITOR_INTERFACE extern "C" __declspec(dllexport)
#endif

using namespace paragon;

namespace
{
	struct TransformComponent
	{
		F32 position[3];
		F32 rotation[3];
		F32 scale[3];

		transform::InitInfo ToInitInfo()
		{
			using namespace DirectX;
			transform::InitInfo info;

			memcpy(&info.position[0], &position[0], sizeof(F32) * _countof(position));
			memcpy(&info.scale[0], &scale[0], sizeof(F32) * _countof(scale));

			XMFLOAT3A rot{ &rotation[0] };
			XMVECTOR quat{ XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3A(&rot))};

			XMFLOAT4A rotQuat{};
			XMStoreFloat4A(&rotQuat, quat);
			memcpy(&info.rotation[0], &rotQuat.x, sizeof(F32) * _countof(info.rotation));

			return info;
		}
	};

	struct GameEntityDesc
	{
		TransformComponent transform;
	};

	game_entity::Entity EntityFromID(id::IDType id)
	{
		return game_entity::Entity(game_entity::EntityID{ id });
	}
}

EDITOR_INTERFACE
id::IDType CreateGameEntity(GameEntityDesc* pDesc)
{
	assert(pDesc);
	GameEntityDesc& desc{ *pDesc };

	transform::InitInfo transformInfo{ desc.transform.ToInitInfo() };
	game_entity::EntityInfo entityInfo{ &transformInfo };

	return game_entity::CreateGameEntity(entityInfo).GetID();
}

EDITOR_INTERFACE
void RemoveGameEntity(id::IDType id)
{
	assert(id::IsValid(id));
	game_entity::RemoveGameEntity(EntityFromID(id));
}