/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#include "resource/texture.h"

namespace quasi_game_engine {

Texture::Texture(unsigned char* pixel_map, int width, int height, Asset asset)
    : ResourceBase(asset), pixel_map_(pixel_map), width_(width),
      height_(height) {}


void Texture::LoadToGraphics(Renderer* render_manager) {
  render_manager->LoadImage(pixel_map_, width_, height_, true, &texture_id_);
}

}  // namespace quasi_game_engine
