/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_OBJECT_LOADER_H
#define QGE_OBJECT_LOADER_H

#include "resource/resource.h"
#include "resource/resource_base.h"
#include "utils/logging/logger.h"
#include "utils/qge_array.h"

#include <assimp/scene.h>

namespace quasi_game_engine {

class ObjectLoader {
/*------------------------------------------------------------------------------
  Class dedicated to load resource information from files. This class should
  only be used by the resource manager
Notes:
- May need to start the assimp logger to get more information out
- Still need to load bones, animations?, tangents, bitangents colours, material,
  method?, primitive type
- Need to read component numbers from file if possible
------------------------------------------------------------------------------*/
 public:
  explicit ObjectLoader(Logger* logger) : logger_(logger) {}
  ~ObjectLoader() = default;

  Resource* Load(Asset asset);

 protected:
  Logger* logger_;

  //Currently can be used to load verticies, normals and texture coordinates
  //Data_in should be an array of required attribute (e.g. mesh->mVertices)
  QgeArray<float> LoadAttribute(const aiVector3D* data_in, int num_vertices,
      int num_components);
  QgeArray<int> LoadFaces(const aiFace* data_in, int num_faces,
      int num_indices);

};

}  // namespace quasi_game_engine

#endif  // QGE_OBJECT_LOADER_H
