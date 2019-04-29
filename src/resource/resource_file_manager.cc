/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#include "resource/resource_file_manager.h"

namespace quasi_game_engine {

ResourceFileManager::ResourceFileManager() {
  // Eventually read from a config file
  freeimg_extensions_ = QgeArray<const char*>(2);
  freeimg_extensions_[0] = "dds";
  freeimg_extensions_[1] = "jpg";
  assimp_extensions_ = QgeArray<const char*>(2);
  assimp_extensions_[0] = "obj";
  assimp_extensions_[1] = "3ds";
}

Resource* ResourceFileManager::LoadFile(Asset asset) {
  // determine what the file is and then call the relevant function
  if (file_utils::CompareFileType(asset.file_location, freeimg_extensions_)) {
    return image_loader.Load(asset);
  } else if (file_utils::CompareFileType(asset.file_location,
                                         assimp_extensions_))
    // return LoadObject(asset);
    return object_loader.Load(asset);

  // assert that this file type is not supported
  LOG(ERROR, RESOURCE) << "The " << asset.file_location
                       << " file type is not supported";

  return NULL;
}

}  // namespace quasi_game_engine
