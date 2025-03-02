#include <fstream>
#include <Windows.h>
#include <filesystem>
#include "res_content_loader.h"
#include "..\game_systems\gs_entity.h"
#include "..\game_systems\gs_transform.h"
#include "..\game_systems\gs_script.h"


#ifndef SHIPPING

namespace
{
	enum gsCOMPONENT_TYPE
	{
		TRANSFORM,
		SCRIPT,

		COUNT
	};


	dsVECTOR<gsENTITY> entities;
	gsTRANSFORM_INIT_INFO transformInfo;
	gsSCRIPT_INIT_INFO scriptInfo;


	const U32 ReadFromBuffer(const U8*& pBufferData)
	{
		const U32 data = *pBufferData;
		pBufferData += sizeof(U32);

		return data;
	}


	bool ReadTransform(const U8*& data, gsENTITY_INFO& info)
	{
		using namespace DirectX;
		F32 rotation[3];

		assert(!info.transform);

		memcpy(&transformInfo.position[0], data, sizeof(transformInfo.position));
		data += sizeof(transformInfo.position);

		memcpy(&transformInfo.scale[0], data, sizeof(transformInfo.scale));
		data += sizeof(transformInfo.scale);

		memcpy(&rotation[0], data, sizeof(rotation));
		data += sizeof(rotation);

		XMFLOAT3A rot(&rotation[0]);
		XMVECTOR quat(XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3A(&rot)));
		XMFLOAT4A rotQuat;
		XMStoreFloat4A(&rotQuat, quat);

		memcpy(&transformInfo.rotation[0], &rotQuat.x, sizeof(transformInfo.rotation));
	
		info.transform = &transformInfo;
		return true;
	}


	bool ReadScript(const U8*& data, gsENTITY_INFO& info)
	{
		assert(!info.script);

		const U32 nameLength = *data;
		data += sizeof(U32);

		if (!nameLength)
		{
			return false;
		}

		// if a script name is longer than 255 characters then something is probably
		// wrong, either with the binary writer or the gameplay programmer
		assert(nameLength < 256);
		char scriptName[256];
		
		memcpy(&scriptName[0], data, nameLength);
		data += nameLength;

		scriptName[nameLength] = 0;
		scriptInfo.scriptCreator = apiGetScriptCreator(dsSTRING_HASH()(scriptName));
	
		info.script = &scriptInfo;
		return scriptInfo.scriptCreator != nullptr;
	}



	bool SetDirectory()
	{
		wchar_t path[MAX_PATH];
		const U32 length = GetModuleFileName(0, &path[0], MAX_PATH);
		if (!length || GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			return false;
		}

		std::filesystem::path p(path);
		SetCurrentDirectory(p.parent_path().wstring().c_str());

		return true;
	}


	using resCOMPONENT_READER = bool(*)(const U8*&, gsENTITY_INFO&);
	resCOMPONENT_READER componentReaders[]
	{
		ReadTransform,
		ReadScript
	};


	static_assert(_countof(componentReaders) == gsCOMPONENT_TYPE::COUNT);
}


bool resLoadGame()
{
	if (!SetDirectory())
	{
		return false;
	}

	std::ifstream gameStream("game.bin", std::ios::in | std::ios::binary);
	dsVECTOR<U8> buffer(std::istreambuf_iterator<char>(gameStream), {});
	assert(buffer.size());

	const U8* bufferPtr = buffer.data();
	
	const U32 numEntities = ReadFromBuffer(bufferPtr);
	if (!numEntities)
	{
		return false;
	}

	for (U32 entityIdx = 0; entityIdx < numEntities; ++entityIdx)
	{
		gsENTITY_INFO info;
		const U32 entityType = ReadFromBuffer(bufferPtr);
		
		const U32 numComponents = ReadFromBuffer(bufferPtr);
		if (!numComponents)
		{
			return false;
		}

		for (U32 componentIdx = 0; componentIdx < numComponents; ++componentIdx)
		{
			const U32 componentType = ReadFromBuffer(bufferPtr);
			assert(componentType < gsCOMPONENT_TYPE::COUNT);

			if (!componentReaders[componentType](bufferPtr, info))
			{
				return false;
			}
		}

		assert(info.transform);
		gsENTITY entity(gsCreateGameEntity(info));
		if (!entity.IsValid())
		{
			return false;
		}

		entities.emplace_back(entity);
	}

	assert(bufferPtr == buffer.data() + buffer.size());
	return true;
}


void resUnloadGame()
{
	for (auto entity : entities)
	{
		gsRemoveGameEntity(entity.GetID());
	}
}


#endif