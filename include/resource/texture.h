/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_TEXTURE_H
#define QGE_TEXTURE_H

#include "renderer/renderer.h"
#include "resource/resource_base.h"

namespace quasi_game_engine {

class Texture : public ResourceBase {
  /*------------------------------------------------------------------------------
    Container class for texture information. Also contains functions to load
  data to graphics and process image Notes:
  - Could include function to unload from graphics
  - Add additional funcrionality such as decompression, gamma
  - define config of image
    - built in mipmap, number of mipmap levels
    - filters (enum) - linear, bilinear, etc
    - texture wrapping
    - texture type (enum) - 2d, 3d, cubemap, etc.
    - pixel type (enum) - RGB, RGBA, etc.
    - bool compressed
  ---------------------------------------------------------------------------*/
 public:
  Texture(unsigned char* pixel_map, int width, int height, Asset asset);
  ~Texture() = default;

  int GetResource() const final { return texture_id_; };

  virtual void LoadToGraphics(Renderer* render_manager);

 private:
  unsigned char* pixel_map_;
  int width_, height_, depth_;
  int texture_id_;
};

}  // namespace quasi_game_engine

#endif  // QGE_TEXTURE_H
