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
                  // NOTT: This value represents the cosine of the angle between normals
                  F32 n = 0.0f;
                  XMVECTOR n2 = XMLoadFloat3(&mesh.normals[refs[k]]);
                  if (!isSoftEdge)
                  {
                     // NOTE: we're accounting for the lenght of the nq in this calculation because
                     //       it can possibly change in this loop iteration. We assume unit length
                     //       for u2.
                     //       cos(angle) == dot(n1, n2) / (||n1|| * ||n2||)
                     XMStoreFloat(&n, XMVector3Dot(n1, n2) * XMVector3ReciprocalLength(n1));
                  }

                  if (isSoftEdge || n > cosAngle)
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
      assert(mesh.rawIndices.size() % 3  == 0);
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
}