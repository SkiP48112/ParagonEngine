#pragma once
#include "..\at_common.h"


struct geomIMPORT_SETTINGS
{
   F32 smoothingAngle;
   U8  calculateNormals;
   U8  calculateTangents;
   U8  reverseHandedness;
   U8  importEmbadedTextures;
   U8  importAnimations;
};


struct atMESH
{
   dsVECTOR<mVECTOR3> positions;
   dsVECTOR<mVECTOR3> normals;
   dsVECTOR<mVECTOR4> tangents;

   dsVECTOR<dsVECTOR<mVECTOR2>> uvSets;

   dsVECTOR<U32> rawIndices;
};


struct geomLOD_GROUP
{
   dsSTRING name;
   dsVECTOR<atMESH> meshes;
};


struct scnSCENE
{
   dsSTRING name;
   dsVECTOR<geomLOD_GROUP> lodGroups;
};


struct scnDATA
{
   U8* buffer;
   U32 bufferSize;
   geomIMPORT_SETTINGS settings;
};