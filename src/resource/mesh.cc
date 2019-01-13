/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#include "resource/mesh.h"

#include "utils/logging/log_capture.h"

namespace quasi_game_engine {

// Need to benchmark if this or just passing a reference would be quicker
//TODO: need to change new to memory manager allocate
Mesh::Mesh(QgeArray<float> verts, QgeArray<float> norms, QgeArray<float> uvs,
           QgeArray<int> faces, Asset asset)
    : ResourceBase(asset), verts_(verts), norms_(norms), uvs_(uvs),
      faces_(faces), vao_(0) {}


void Mesh::LoadToGraphics(Renderer* render_manager) {
  LOG(INFO, RESOURCE) << "Loading mesh for '" << asset_.name
      << "' in '" << asset_.file_location << "' to graphics";

  render_manager->FillVao(&vao_, [=]() {
    LOG(INFO, RESOURCE) << "Loading verticies to graphics";
    render_manager->LoadVertexAttribute(verts_);
    render_manager->SetAttributePointer(
        AttributeMetadata(ATTR_POSITION, verts_.GetNumComponents(), 
                GL_FLOAT, 0, false), 0
    );

    LOG(INFO, RESOURCE) << "Loading normals to graphics";
    render_manager->LoadVertexAttribute(norms_);
    render_manager->SetAttributePointer(
        AttributeMetadata(ATTR_NORMAL, norms_.GetNumComponents(), 
            GL_FLOAT, 0, false), 0
    );

    LOG(INFO, RESOURCE) << "Loading texture coordinates to graphics";
    render_manager->LoadVertexAttribute(uvs_);
    render_manager->SetAttributePointer(
        AttributeMetadata(ATTR_UV, uvs_.GetNumComponents(), 
            GL_FLOAT, 0, false), 0
    );

    LOG(INFO, RESOURCE) << "Loading VBO indices to graphics";
    render_manager->LoadVertexIndices(faces_);
  });
}

}  // namespace quasi_game_engine
