/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#include "resource/resource_base.h"

#include "utils/logging/log_capture.h"

namespace quasi_game_engine {


ResourceBase::ResourceBase(Asset asset) : asset_(asset) {}

void ResourceBase::LoadToGraphics(Renderer* render_manager) {
  LOG(ERROR, RESOURCE) 
      << "This resource cannot be loaded to graphics!";
}

}  // namespace quasi_game_engine
