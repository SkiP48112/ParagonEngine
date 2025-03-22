#include "at_primitive_mesh.h"
#include "geom/geom_geometry.h"


namespace{
   
   using atPRIMITIVE_MESH_CREATOR = void(*)(scnSCENE&, const atPRIMITIVE_MESH_INIT_INFO&);


   void atCreatePlane(scnSCENE&, const atPRIMITIVE_MESH_INIT_INFO&);
   void atCreateCube(scnSCENE&, const atPRIMITIVE_MESH_INIT_INFO&);
   void atCreateUvSphere(scnSCENE&, const atPRIMITIVE_MESH_INIT_INFO&);
   void atCreateIcoSphere(scnSCENE&, const atPRIMITIVE_MESH_INIT_INFO&);
   void atCreateCylinder(scnSCENE&, const atPRIMITIVE_MESH_INIT_INFO&);
   void atCreateCapsule(scnSCENE&, const atPRIMITIVE_MESH_INIT_INFO&);


   atPRIMITIVE_MESH_CREATOR creators[]
   {
      atCreatePlane,
      atCreateCube,
      atCreateUvSphere,
      atCreateIcoSphere,
      atCreateCylinder,
      atCreateCapsule
   };

   static_assert(_countof(creators) == atPRIMITIVE_MESH_TYPE::COUNT);


   struct atAXIS
   {
      enum : U32
      {
         X = 0,
         Y = 1,
         Z = 2
      };
   };


   geomMESH atCreatePlane(
      const atPRIMITIVE_MESH_INIT_INFO& info,
      const U32 horizontalIndex = atAXIS::X,
      const U32 verticalIndex = atAXIS::Z,
      const bool flipWinding = false,
      const mVECTOR3 offset = { -0.5f, 0.0f, -0.5f },
      const mVECTOR2 uRange = { 0.0f, 1.0f },
      const mVECTOR2 vRange = { 0.0f, 1.0f })
   {
      assert(horizontalIndex < 3 && verticalIndex < 3);
      assert(horizontalIndex != verticalIndex);

      const U32 horizontalCount = mClamp(info.segments[horizontalIndex], 1U, 10U);
      const U32 verticalCount = mClamp(info.segments[verticalIndex], 1U, 10U);

      const F32 horizontalStep = 1.0f / horizontalCount;
      const F32 verticalStep = 1.0f / verticalCount;

      const F32 uStep = (uRange.y - uRange.x) / horizontalCount;
      const F32 vStep = (vRange.y - vRange.x) / verticalCount;

      geomMESH mesh;
      dsVECTOR<mVECTOR2> uvs;

      for (U32 j = 0; j <= verticalCount; ++j)
      {
         for (U32 i = 0; i <= horizontalCount; ++i)
         {
            mVECTOR3 position(offset);
            F32* const asArray = &position.x;

            asArray[horizontalIndex] += i * horizontalStep;
            asArray[verticalIndex] += j * verticalStep;

            mesh.positions.emplace_back(position.x * info.size.x, position.y * info.size.y, position.z * info.size.z);

            mVECTOR2 uv{ uRange.x, 1.0f - vRange.x };

            uv.x += i * uStep;
            uv.y -= j * vStep;

            uvs.emplace_back(uv);
         }
      }

      assert(mesh.positions.size() == (((U64)horizontalCount + 1) * ((U64)verticalCount + 1)));

      const U32 rowLength = horizontalCount + 1;
      for (U32 j = 0; j < verticalCount; ++j)
      {
         U32 k = 0;
         for (U32 i = k; i < horizontalCount; ++i)
         {
            const U32 index[4]
            {
               i + j * rowLength,
               i + (j + 1) * rowLength,
               (i + 1) + j * rowLength,
               (i + 1) + (j + 1) * rowLength
            };

            mesh.rawIndices.emplace_back(index[0]);
            mesh.rawIndices.emplace_back(index[flipWinding ? 2 : 1]);
            mesh.rawIndices.emplace_back(index[flipWinding ? 1 : 2]);

            mesh.rawIndices.emplace_back(index[2]);
            mesh.rawIndices.emplace_back(index[flipWinding ? 3 : 1]);
            mesh.rawIndices.emplace_back(index[flipWinding ? 1 : 3]);
         }
         ++k;
      }

      const U32 numIndices = 3 * 2 * horizontalCount * verticalCount;
      assert(mesh.rawIndices.size() == numIndices);

      mesh.uvSets.resize(1);
      for (U32 i = 0; i < numIndices; ++i)
      {
         mesh.uvSets[0].emplace_back(uvs[mesh.rawIndices[i]]);
      }

      return mesh;
   }


   void atCreatePlane(scnSCENE& scene, const atPRIMITIVE_MESH_INIT_INFO& info)
   {
      geomLOD_GROUP lodGroup;
      lodGroup.name = "plane";
      lodGroup.meshes.emplace_back(atCreatePlane(info));

      scene.lodGroups.emplace_back(lodGroup);
   }


   void atCreateCube(scnSCENE&, const atPRIMITIVE_MESH_INIT_INFO& info)
   {

   }


   void atCreateUvSphere(scnSCENE&, const atPRIMITIVE_MESH_INIT_INFO& info)
   {

   }


   void atCreateIcoSphere(scnSCENE&, const atPRIMITIVE_MESH_INIT_INFO& info)
   {

   }


   void atCreateCylinder(scnSCENE&, const atPRIMITIVE_MESH_INIT_INFO& info)
   {

   }


   void atCreateCapsule(scnSCENE&, const atPRIMITIVE_MESH_INIT_INFO& info)
   {

   }
}


EDITOR_INTERFACE
void CreatePrimitiveMesh(scnDATA* data, atPRIMITIVE_MESH_INIT_INFO* info)
{
   assert(data && info);
   assert(info->type < atPRIMITIVE_MESH_TYPE::COUNT);
   if (info->type > atPRIMITIVE_MESH_TYPE::COUNT)
   {
      return;
   }

   scnSCENE scene;
   creators[info->type](scene, *info);

   data->settings.calculateNormals = 1;

   scnProcessScene(scene, data->settings);
   scnPackData(scene, *data);
}