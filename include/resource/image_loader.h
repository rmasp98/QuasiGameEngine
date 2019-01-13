/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_IMAGE_LOADER_H
#define QGE_IMAGE_LOADER_H

#include "resource/resource.h"
#include "resource/resource_base.h"
#include "utils/data_types/qge_array.h"

namespace quasi_game_engine {

class ImageLoader {
/*------------------------------------------------------------------------------
  This class is dedicated towards loading images from files using FreeImage
Notes:
-
---------------------------------------------------------------------------*/
 public:
  ImageLoader();
  ~ImageLoader();

  Resource* Load(Asset asset);
};

}  // namespace quasi_game_engine

#endif  // QGE_IMAGE_LOADER_H
