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


struct geomPACKED_VERTEX_STATIC
{
   mVECTOR3 position;
   U8 reserved[3];
   U8 tSign; // bit 0: tangent handedness * (tangent.z sign), bit 1: normal.z sign (0 means -, 1 means +)

   U16 normal[2];
   U16 tangent[2];
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

   // Output data
   dsSTRING name;
   dsVECTOR<geomPACKED_VERTEX_STATIC> packedVerticesStatic;
   F32 lodTreshhold = -1.0f;
   F32 lodId = U32_INVALID_ID;
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