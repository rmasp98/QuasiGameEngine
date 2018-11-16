/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_OPENGL_RENDERER_H
#define QGE_OPENGL_RENDERER_H

#include "renderer/renderer.h"
#include "utils/qge_array.h"
#include "utils/logging/logger.h"

//Graphics libraries
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace quasi_game_engine {
class LogWorker;

class OpenGLRenderer : public Renderer {
/*------------------------------------------------------------------------------
  Description
Notes
- The should be a parent class with child classes for opengl and directx
------------------------------------------------------------------------------*/
 public:
  explicit OpenGLRenderer(LogWorker* log_worker);
  ~OpenGLRenderer() = default;

  //Getting rid of copy/move constructors/assignment operators (may need later)
  OpenGLRenderer(const OpenGLRenderer&) = delete;
  OpenGLRenderer& operator=(const OpenGLRenderer&) = delete;
  OpenGLRenderer(OpenGLRenderer&&) = delete;
  OpenGLRenderer& operator=(OpenGLRenderer&&) = delete;

  bool InitGraphics() final;
  void Draw() final;
  bool LoadImage(const unsigned char *pixel_map, int width, int height,
                 bool is_mipmap, int* texture_id) final;
  bool LoadVertexAttribute(const QgeArray<float> attribute_data,
                           int attribute_index, int* vao) final;

  bool LoadVertexIndices(const QgeArray<int> indices, int* vao) final;

 private:
  Logger logger_;
};

} // namespace quasi_game_engine

#endif // QGE_OPENGL_RENDERER_H
