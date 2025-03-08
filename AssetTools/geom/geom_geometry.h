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


struct geomVERTEX
{
   mVECTOR4 tangent;
   mVECTOR3 position;
   mVECTOR3 normal;
   mVECTOR2 uv;
};


struct geomMESH
{
   // Initial data
   dsVECTOR<mVECTOR3> positions;
   dsVECTOR<mVECTOR3> normals;
   dsVECTOR<mVECTOR4> tangents;

   dsVECTOR<dsVECTOR<mVECTOR2>> uvSets;

   dsVECTOR<U32> rawIndices;

   // Intermediate data
   dsVECTOR<geomVERTEX> vertices;
   dsVECTOR<U32> indices;
};


struct geomLOD_GROUP
{
   dsSTRING name;
   dsVECTOR<geomMESH> meshes;
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


void scnProcessScene(scnSCENE& scene, const geomIMPORT_SETTINGS& settings);
void scnPackData(const scnSCENE& scene, scnDATA& data);