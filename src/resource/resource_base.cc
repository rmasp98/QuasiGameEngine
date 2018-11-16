/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#include "resource/resource_base.h"

namespace quasi_game_engine {

//Allows sharing of logger across all resources by passing it once (static variable)
Logger* ResourceBase::logger_;

ResourceBase::ResourceBase(Asset asset) : asset_(asset) {
  if (logger_ == NULL)
    LOG(LOG_ERROR, logger_) << "Logger has not been set on this resource class. "
                            << "This means either you have altered "
                            << "ResourceManager or not loaded this resource "
                            << "from it.";
}

void ResourceBase::LoadToGraphics(Renderer* render_manager) {
  LOG(LOG_ERROR, logger_) << "This resource cannot be loaded to graphics!";
}

}  // namespace quasi_game_engine
