#include "common.h"
#include "app_common_headers.h"
#include "id.h"
#include "..\Engine\game_entities\ge_entity.h"
#include "..\Engine\game_entities\ge_transform.h"
#include "..\Engine\game_entities\ge_script.h"


namespace
{
	struct apiTRANSFORM_COMPONENT
	{
		geTRANSFORM_INIT_INFO ToInitInfo()
		{
			using namespace DirectX;
			geTRANSFORM_INIT_INFO info;

			memcpy(&info.position[0], &position[0], sizeof(position));
			memcpy(&info.scale[0], &scale[0], sizeof(scale));

			XMFLOAT3A rot{ &rotation[0] };
			XMVECTOR quat{ XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3A(&rot)) };

			XMFLOAT4A rotQuat{};
			XMStoreFloat4A(&rotQuat, quat);
			memcpy(&info.rotation[0], &rotQuat.x, sizeof(info.rotation));

			return info;
		}

		F32 position[3];
		F32 rotation[3];
		F32 scale[3];
	};


	struct apiSCRIPT_COMPONENT
	{
		geSCRIPT_CREATOR scriptCreator;

		geSCRIPT_INIT_INFO ToInitInfo()
		{
			geSCRIPT_INIT_INFO info;
			info.scriptCreator = scriptCreator;
			return info;
		}
	};


	struct apiGAME_ENTITY_DESC
	{
		apiTRANSFORM_COMPONENT transform;
		apiSCRIPT_COMPONENT script;
	};


	geENTITY EntityFromID(idID_TYPE id)
	{
		return geENTITY(geENTITY_ID{ id });
	}
}


EDITOR_INTERFACE
idID_TYPE CreateGameEntity(apiGAME_ENTITY_DESC* pDesc)
{
	assert(pDesc);
	apiGAME_ENTITY_DESC& desc{ *pDesc };

	geTRANSFORM_INIT_INFO transformInfo{ desc.transform.ToInitInfo() };
	geSCRIPT_INIT_INFO scriptInfo{ desc.script.ToInitInfo() };

	geENTITY_INFO entityInfo
	{ 
		&transformInfo,
		&scriptInfo
	};

	return geCreateGameEntity(entityInfo).GetID();
}


EDITOR_INTERFACE
void RemoveGameEntity(idID_TYPE id)
{
	assert(idIsValid(id));
	geRemoveGameEntity(geENTITY_ID{ id });
}