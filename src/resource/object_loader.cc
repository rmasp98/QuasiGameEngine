/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#include "resource/object_loader.h"
#include "resource/mesh.h"

#include "utils/logging/log_capture.h"

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

namespace quasi_game_engine {

Resource* ObjectLoader::Load(Asset asset) {
  LOG(TRACE, RESOURCE) 
      << "Loading object from " << asset.file_location;

  Assimp::Importer importer;

  //Read from file with postproccessing (calculate tangents and bitangents|
  // make all faces triangles | remove duplicate verts |
  // sort by number of points in face)
  const aiScene* scene = importer.ReadFile(
      asset.file_location, aiProcess_CalcTangentSpace | aiProcess_Triangulate |
                               aiProcess_JoinIdenticalVertices |
                               aiProcess_SortByPType);

  //Checks to see if file successfully loaded
  if (scene) {
    for (size_t i_mesh = 0; i_mesh < scene->mNumMeshes; ++i_mesh) {
      const aiMesh* mesh = scene->mMeshes[i_mesh];
      //Check if current mesh is the one we want to lead
      if (!strcmp(mesh->mName.C_Str(), asset.name)) {
        //This should initialise all possible outputs
        QgeArray<float> verticies, normals, texture_coords;
        QgeArray<int> faces;

        if (mesh->HasFaces()) {
          LOG(TRACE, RESOURCE) 
              << "Getting faces from '" << asset.name
              << "', in '" << asset.file_location << "'";

          faces = LoadFaces(mesh->mFaces, mesh->mNumFaces,
              mesh->mFaces[0].mNumIndices);
        }

        if (mesh->HasPositions()) {
          LOG(TRACE, RESOURCE)
              << "Getting verticies from '" << asset.name << "', in '"
              << asset.file_location << "'";

          verticies = LoadAttribute(mesh->mVertices, mesh->mNumVertices, 3);
        }

        if (mesh->HasNormals()) {
          LOG(TRACE, RESOURCE)
              << "Getting normals from '" << asset.name << "', in '"
              << asset.file_location << "'";

          normals = LoadAttribute(mesh->mNormals, mesh->mNumVertices, 3);
        }

        if (mesh->HasTextureCoords(0)) {
          LOG(TRACE, RESOURCE)
              << "Getting texture coordinates from '" << asset.name
              << "', in '" << asset.file_location << "'";

          texture_coords = LoadAttribute(mesh->mTextureCoords[0],
              mesh->mNumVertices, *mesh->mNumUVComponents);
        }

        //This will need to be altered for all possible combination of attribs
        Resource* new_resource =
            new Mesh(verticies, normals, texture_coords, faces, asset);

        return new_resource;
      }
    }
  }

  LOG(TRACE, RESOURCE) 
      << "Failed to load " << asset.file_location
      << ". Either AssImp does not support this filetype"
      << ", the file is corrupt or does not exist";

  return NULL;
}


QgeArray<float> ObjectLoader::LoadAttribute(const aiVector3D* data_in,
    int num_vertices, int num_components) {

  QgeArray<float> data_out(num_vertices * num_components, num_components);
  int offset = 0;
  for (size_t i_vert = 0; i_vert < num_vertices; ++i_vert) {
    for (size_t j_comp = 0; j_comp < num_components; ++j_comp) {
      data_out[offset + j_comp] = data_in[i_vert][j_comp];
    }
    offset += num_components;
  }
  return data_out;
}


QgeArray<int> ObjectLoader::LoadFaces(const aiFace* faces_in, 
    int num_faces, int num_indices) {
  QgeArray<int> faces_out(num_faces * num_indices, num_indices);
  int offset = 0;
  for (size_t i_face = 0; i_face < num_faces; ++i_face) {
    for (size_t j_index = 0; j_index < num_indices; ++j_index) {
      faces_out[offset + j_index] = faces_in[i_face].mIndices[j_index];
    }
    offset += num_indices;
  }
  return faces_out;
}

}  // namespace quasi_game_engine
