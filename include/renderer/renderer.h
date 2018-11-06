/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_RENDERER_H
#define QGE_RENDERER_H

#include "utils/qge_array.h"

namespace quasi_game_engine {

class Renderer {
  /*------------------------------------------------------------------------------
    Interface for graphics API wrappers
  Notes
  - The should be a parent class with child classes for opengl and directx
  ------------------------------------------------------------------------------*/
 public:
  virtual ~Renderer() = default;

  // Getting rid of copy/move constructors/assignment operators (may need later)
  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;
  Renderer(Renderer&&) = delete;
  Renderer& operator=(Renderer&&) = delete;

  virtual bool InitGraphics() = 0;
  virtual void Draw() = 0;

  virtual bool LoadImage(const unsigned char* pixel_map, int width, int height,
                         bool is_mipmap, int* texture_id) = 0;

  virtual bool LoadVertexAttribute(const QgeArray<float> attribute_data,
                                   int attribute_index, int* vao) = 0;

  virtual bool LoadVertexIndices(const QgeArray<int> indices, int* vao) = 0;

 protected:
  Renderer() = default;
};

}  // namespace quasi_game_engine

#endif  // QGE_RENDERER_H
