/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_RESOURCE_FILE_MANAGER_H
#define QGE_RESOURCE_FILE_MANAGER_H

#include "utils/file_manager.h"
#include "resource/object_loader.h"
#include "resource/image_loader.h"
#include "resource/resource.h"
#include "resource/resource_base.h"
#include "utils/logging/logger.h"
#include "utils/qge_array.h"

namespace quasi_game_engine {

class ResourceFileManager : public FileManager {
/*------------------------------------------------------------------------------
  Class dedicated to load resource information from files. This class should
  only be used by the resource manager
Notes:
-
---------------------------------------------------------------------------*/
 public:
  ResourceFileManager(Logger* logger);
  ~ResourceFileManager() = default;

  Resource* LoadFile(Asset asset);

  //GUID is a unique number used to search for resource
  int CalcGUID(const char* file_location, const char* asset_name);

 protected:
  QgeArray<const char*> freeimg_extensions_;
  QgeArray<const char*> assimp_extensions_;
  Logger* logger_;

  //Uses FreeImage to load the pixelmap from an image file
  ImageLoader image_loader;
  //Uses AssImp to load the meshs, etc. from aa variety of files
  ObjectLoader object_loader;
};
}  // namespace quasi_game_engine

#endif  // QGE_RESOURCE_FILE_MANAGER_H
