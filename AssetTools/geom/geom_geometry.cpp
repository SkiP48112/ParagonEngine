#include "geom_geometry.h"


namespace
{
   using namespace DirectX;


   void geomRecalculateNormals(geomMESH& mesh)
   {
      const U32 numIndices = (U32)mesh.rawIndices.size();
      mesh.normals.reserve(numIndices);

      for (U32 i = 0; i < numIndices; ++i)
      {
         const U32 i0 = mesh.rawIndices[i];
         const U32 i1 = mesh.rawIndices[++i];
         const U32 i2 = mesh.rawIndices[++i];

         XMVECTOR v0 = XMLoadFloat3(&mesh.positions[i0]);
         XMVECTOR v1 = XMLoadFloat3(&mesh.positions[i1]);
         XMVECTOR v2 = XMLoadFloat3(&mesh.positions[i2]);

         XMVECTOR e0 = v1 - v0;
         XMVECTOR e1 = v2 - v0;

         XMVECTOR normal = XMVector3Normalize(XMVector3Cross(e0, e1));

         XMStoreFloat3(&mesh.normals[i], normal);
         mesh.normals[i - 1] = mesh.normals[i];
         mesh.normals[i - 2] = mesh.normals[i];
      }
   }


   void geomProcessNormals(geomMESH& mesh, F32 smoothingAngle)
   {
      const F32 cosAngle = XMScalarCos(M_PI - smoothingAngle * M_PI / 180.0f);
      const bool isHardEdge = XMScalarNearEqual(smoothingAngle, 180.0f, M_EPSILON);
      const bool isSoftEdge = XMScalarNearEqual(smoothingAngle, 0.0f, M_EPSILON);

      const U32 numIndices = (U32)mesh.rawIndices.size();
      const U32 numVertices = (U32)mesh.positions.size();
      assert(numIndices && numVertices);

      mesh.indices.resize(numIndices);

      dsVECTOR<dsVECTOR<U32>> idxRef(numVertices);
      for (U32 i = 0; i < numIndices; ++i)
      {
         idxRef[mesh.rawIndices[i]].emplace_back(i);
      }

      for (U32 i = 0; i < numVertices; ++i)
      {
         dsVECTOR<U32>& refs = idxRef[i];
         U32 numRefs = (U32)refs.size();

         for (U32 j = 0; j < numRefs; ++j)
         {
            mesh.indices[refs[j]] = (U32)mesh.vertices.size();
            geomVERTEX& vertex = mesh.vertices.emplace_back();
            vertex.position = mesh.positions[mesh.rawIndices[refs[j]]];

            XMVECTOR n1 = XMLoadFloat3(&mesh.normals[refs[j]]);
            if (!isHardEdge)
            {
               for (U32 k = j + 1; k < numRefs; ++k)
               {
                  F32 cosBetweenNormals = 0.0f;
                  XMVECTOR n2 = XMLoadFloat3(&mesh.normals[refs[k]]);
                  if (!isSoftEdge)
                  {
                     // NOTE: we're accounting for the lenght of the nq in this calculation because
                     //       it can possibly change in this loop iteration. We assume unit length
                     //       for u2.
                     //       cos(angle) == dot(n1, n2) / (||n1|| * ||n2||)
                     XMStoreFloat(&cosBetweenNormals, XMVector3Dot(n1, n2) * XMVector3ReciprocalLength(n1));
                  }

                  if (isSoftEdge || cosBetweenNormals > cosAngle)
                  {
                     n1 + n2;

                     mesh.indices[refs[k]] = mesh.indices[refs[j]];
                     refs.erase(refs.begin() + k);

                     --numRefs;
                     --k;
                  }
               }
            }

            XMStoreFloat3(&vertex.normal, XMVector3Normalize(n1));
         }
      }
   }


   void geomProcessUVs(geomMESH& mesh)
   {
      dsVECTOR<geomVERTEX> oldVertices;
      oldVertices.swap(mesh.vertices);

      dsVECTOR<U32> oldIndices(mesh.indices.size());
      oldIndices.swap(mesh.indices);

      const U32 numVertices = (U32)oldVertices.size();
      const U32 numIndices = (U32)oldIndices.size();

      dsVECTOR<dsVECTOR<U32>> idxRef(numVertices);
      for (U32 i = 0; i < numIndices; ++i)
      {
         idxRef[oldIndices[i]].emplace_back(i);
      }

      for (U32 i = 0; i < numVertices /* numIndices??? */; ++i)
      {
         dsVECTOR<U32>& refs = idxRef[i];
         U32 numRefs = (U32)refs.size();

         for (U32 j = 0; j < numRefs; ++j)
         {
            mesh.indices[refs[j]] = (U32)mesh.vertices.size();
            geomVERTEX& vertex = oldVertices[oldIndices[refs[j]]];
            vertex.uv = mesh.uvSets[0][refs[j]];
            mesh.vertices.emplace_back(vertex);

            for (U32 k = j + 1; k < numRefs; ++k)
            {
               mVECTOR2& uv1 = mesh.uvSets[0][refs[k]];
               if (XMScalarNearEqual(vertex.uv.x, uv1.x, M_EPSILON) && XMScalarNearEqual(vertex.uv.y, uv1.y, M_EPSILON))
               {
                  mesh.indices[refs[k]] = mesh.indices[refs[j]];
                  refs.erase(refs.begin() + k);
                  --numRefs;
                  --k;
               }
            }
         }
      }
   }


   void geomPackVerticesStatic(geomMESH& mesh)
   {
      const U32 numVertices = (U32)mesh.vertices.size();
      assert(numVertices);

      mesh.packedVerticesStatic.reserve(numVertices);
      for (U32 i = 0; i < numVertices; ++i)
      {
         geomVERTEX& vertex = mesh.vertices[i];

         const U8 signs = (U8)((vertex.normal.z > 0.0f) << 1);
         const U16 normalX = (U16)mPackFloat<16>(vertex.normal.x, -1.0f, 1.0f);
         const U16 normalY = (U16)mPackFloat<16>(vertex.normal.y, -1.0f, 1.0f);

         // TODO: pack tangents in sign and in x/y components

         mesh.packedVerticesStatic.emplace_back
         (
            geomPACKED_VERTEX_STATIC
            {
               vertex.position,
               {0, 0, 0},
               signs,
               {normalX, normalY },
               {},
               vertex.uv
            }
         );
      }
   }


   void geomProcessVertices(geomMESH& mesh, const geomIMPORT_SETTINGS& settings)
   {
      assert(mesh.rawIndices.size() % 3 == 0);
      if (settings.calculateNormals || mesh.normals.empty())
      {
         geomRecalculateNormals(mesh);
      }

      geomProcessNormals(mesh, settings.smoothingAngle);

      if (!mesh.uvSets.empty())
      {
         geomProcessUVs(mesh);
      }

      geomPackVerticesStatic(mesh);
   }


   U64 geomGetMeshSize(const geomMESH& mesh)
   {
      const U64 numVertices = mesh.vertices.size();
      const U64 vertexBufferSize = sizeof(geomPACKED_VERTEX_STATIC) * numVertices;

      const U64 indexSize = numVertices < (1 << 16) ? sizeof(16) : sizeof(32);
      const U64 indexBufferSize = indexSize * mesh.indices.size();

      const U64 size
      {
         U32_SIZE +            // name length
         mesh.name.size() +   // room for mesh name string
         U32_SIZE +            // mesh id
         U32_SIZE +            // vertex size
         U32_SIZE +            // number of vertices
         U32_SIZE +            // index size (16 bits or 32 bits)
         U32_SIZE +            // number if indices
         sizeof(F32) +        // LOD threshold
         vertexBufferSize +   // room for vertices
         indexBufferSize      // room for indices
      };

      return size;
   }


   void geomPackMeshData(const geomMESH& mesh, U8* const buffer, U64& bufferIdx)
   {
      U32 storage = 0;

      // mesh name
      storage = (U32)mesh.name.size();
      memcpy(&buffer[bufferIdx], &storage, U32_SIZE);
      bufferIdx += U32_SIZE;

      memcpy(&buffer[bufferIdx], mesh.name.c_str(), storage);
      bufferIdx += storage;

      // LOD id
      storage = mesh.lodId;
      memcpy(&buffer[bufferIdx], &storage, U32_SIZE);
      bufferIdx += U32_SIZE;

      // vertex size
      constexpr U32 vertexSize = sizeof(geomPACKED_VERTEX_STATIC);
      storage = vertexSize;
      memcpy(&buffer[bufferIdx], &storage, U32_SIZE);
      bufferIdx += U32_SIZE;

      // number of vertices
      const U32 numVertices = (U32)mesh.vertices.size();
      storage = numVertices;
      memcpy(&buffer[bufferIdx], &storage, U32_SIZE);
      bufferIdx += U32_SIZE;

      // index size (16 bits or 32 bits)
      const U32 indexSize = (numVertices < (1 << 16)) ? sizeof(U16) : sizeof(U32);
      storage = indexSize;
      memcpy(&buffer[bufferIdx], &storage, U32_SIZE);
      bufferIdx += U32_SIZE;

      // number of indices
      const U32 numIndices = (U32)mesh.indices.size();
      storage = numIndices;
      memcpy(&buffer[bufferIdx], &storage, U32_SIZE);
      bufferIdx += U32_SIZE;

      // LOD threshold
      memcpy(&buffer[bufferIdx], &mesh.lodTreshhold, F32_SIZE);
      bufferIdx += F32_SIZE;

      // vertex data
      storage = vertexSize * numVertices;
      memcpy(&buffer[bufferIdx], mesh.packedVerticesStatic.data(), storage);
      bufferIdx += storage;

      // index data
      storage = indexSize * numIndices;
      void* data = (void*)mesh.indices.data();
      dsVECTOR<U16> indices;

      if (indexSize == U16_SIZE)
      {
         indices.resize(numIndices);
         for (U32 i = 0; i < numIndices; ++i)
         {
            indices[i] = (U16)mesh.indices[i];
         }

         data = (void*)indices.data();
      }

      memcpy(&buffer[bufferIdx], data, storage);
      bufferIdx += storage;
   }


   U64 scnGetSceneSize(const scnSCENE& scene)
   {
      U64 size
      {
         U32_SIZE +               // name length
         scene.name.size() +      // room for scene name string
         U32_SIZE                 // amount of LODs
      };

      for (auto& lod : scene.lodGroups)
      {
         U64 lodSize
         {
            U32_SIZE +            // LOD name length
            lod.name.size() +     // roof for LOD name string
            U32_SIZE              // amount of meshed in current LOD
         };

         for (auto& mesh : lod.meshes)
         {
            lodSize += geomGetMeshSize(mesh);
         }

         size += lodSize;
      }

      return size;
   }
}


void scnProcessScene(scnSCENE& scene, const geomIMPORT_SETTINGS& settings)
{
   for (auto& lod : scene.lodGroups)
   {
      for (auto& mesh : lod.meshes)
      {
         geomProcessVertices(mesh, settings);
      }
   }
}


void scnPackData(const scnSCENE& scene, scnDATA& data)
{
   const U64 sceneSize = scnGetSceneSize(scene);

   data.bufferSize = (U32)sceneSize;
   data.buffer = (U8*)CoTaskMemAlloc(sceneSize);
   assert(data.buffer);

   U8* const buffer = data.buffer;
   U64 bufferIdx = 0;
   U32 storage = 0;

   // scene name
   storage = (U32)scene.name.size();
   memcpy(&buffer[bufferIdx], &storage, U32_SIZE);
   bufferIdx += U32_SIZE;

   memcpy(&buffer[bufferIdx], scene.name.c_str(), storage);
   bufferIdx += storage;

   // number of LODs
   storage = (U32)scene.lodGroups.size();
   memcpy(&buffer[bufferIdx], &storage, U32_SIZE);
   bufferIdx += U32_SIZE;

   for (auto& lod : scene.lodGroups)
   {
      // LOD name
      storage = (U32)lod.name.size();
      memcpy(&buffer[bufferIdx], &storage, U32_SIZE);
      bufferIdx += U32_SIZE;

      memcpy(&buffer[bufferIdx], lod.name.c_str(), storage);
      bufferIdx += storage;

      // number of meshes in current LOD
      storage = (U32)lod.meshes.size();
      memcpy(&buffer[bufferIdx], &storage, U32_SIZE);
      bufferIdx += U32_SIZE;

      for (auto& mesh : lod.meshes)
      {
         geomPackMeshData(mesh, buffer, bufferIdx);
      }
   }

   assert(sceneSize == bufferIdx);
}