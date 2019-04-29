/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_MESH_H
#define QGE_MESH_H

#include "renderer/renderer.h"
#include "resource/resource_base.h"
#include "utils/data_types/qge_array.h"

namespace quasi_game_engine {

class Mesh : public ResourceBase {
  /*------------------------------------------------------------------------------
    Container for mesh data and any auxillary functions to process that data
  Notes:
  - maybe make function to unload from graphics
  ---------------------------------------------------------------------------*/
 public:
  Mesh(QgeArray<float> verts, QgeArray<float> norms, QgeArray<float> uvs,
       QgeArray<int> faces, Asset asset);
  ~Mesh() = default;

  // Getting rid of copy/move constructors/assignment operators (may need later)
  Mesh(const Mesh&) = delete;
  Mesh& operator=(const Mesh&) = delete;
  Mesh(Mesh&&) = delete;
  Mesh& operator=(Mesh&&) = delete;

  int GetResource() const final { return vao_; };

  // Need to make sure that you cannot affect member variables from this
  // Also do we ever need this?
  const QgeArray<float> GetVerticies() const { return verts_; }
  const QgeArray<float> GetNormals() const { return norms_; }
  const QgeArray<float> GetTexCoordinates() const { return uvs_; }
  const QgeArray<int> GetFaces() const { return faces_; }

  // Passes data to GPU and generates vao index
  void LoadToGraphics(Renderer* render_manager);

 private:
  QgeArray<float> verts_, norms_, uvs_;
  QgeArray<int> faces_;
  int vao_;
};

}  // namespace quasi_game_engine

#endif  // QGE_RESOURCE_H
