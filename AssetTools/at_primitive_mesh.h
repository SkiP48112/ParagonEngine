#pragma once

#include "at_common.h"


enum atPRIMITIVE_MESH_TYPE : U32
{
   PMT_PLANE,
   PMT_CUBE,
   PMT_UV_SPHERE,
   PMT_ICO_SPHERE,
   PMT_CYLINDER,
   PMT_CAPSULE,

   COUNT
};


struct atPRIMITIVE_MESH_INIT_INFO
{
   atPRIMITIVE_MESH_TYPE type;

   U32 segments[3]{ 1, 1, 1 };
   U32 lod = 0;

   mVECTOR3 size{ 1, 1, 1 };
};