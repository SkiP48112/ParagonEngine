#include "common.h"
#include "app_common_headers.h"
#include "..\Engine\game_systems\gs_entity.h"
#include "..\Engine\game_systems\gs_transform.h"
#include "..\Engine\game_systems\gs_script.h"


namespace
{
	struct apiTRANSFORM_COMPONENT
	{
		gsTRANSFORM_INIT_INFO ToInitInfo()
		{
			using namespace DirectX;
			gsTRANSFORM_INIT_INFO info;

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
		gsSCRIPT_CREATOR scriptCreator;

		gsSCRIPT_INIT_INFO ToInitInfo()
		{
			gsSCRIPT_INIT_INFO info;
			info.scriptCreator = scriptCreator;
			return info;
		}
	};


	struct apiGAME_ENTITY_DESC
	{
		apiTRANSFORM_COMPONENT transform;
		apiSCRIPT_COMPONENT script;
	};


	gsENTITY EntityFromID(idID_TYPE id)
	{
		return gsENTITY(gsENTITY_ID{ id });
	}
}


EDITOR_INTERFACE
idID_TYPE CreateGameEntity(apiGAME_ENTITY_DESC* pDesc)
{
	assert(pDesc);
	apiGAME_ENTITY_DESC& desc{ *pDesc };

	gsTRANSFORM_INIT_INFO transformInfo{ desc.transform.ToInitInfo() };
	gsSCRIPT_INIT_INFO scriptInfo{ desc.script.ToInitInfo() };

	gsENTITY_INFO entityInfo
	{ 
		&transformInfo,
		&scriptInfo
	};

	return gsCreateGameEntity(entityInfo).GetID();
}


EDITOR_INTERFACE
void RemoveGameEntity(idID_TYPE id)
{
	assert(idIsValid(id));
	gsRemoveGameEntity(gsENTITY_ID{ id });
}