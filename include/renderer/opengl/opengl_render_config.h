/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_OPENGL_RENDER_CONFIG_H
#define QGE_OPENGL_RENDER_CONFIG_H

#include "renderer/render_config.h"
#include "utils/logging/logger.h"

#include <GL/glew.h>

namespace quasi_game_engine {

class OpenGLRenderConfig : public RenderConfig {
/*------------------------------------------------------------------------------
  This class stores and applys the configuration required to properly render
  particular scenes
Notes
- This overall feels a bit shit. Need to find a better way!
------------------------------------------------------------------------------*/
 public:
  OpenGLRenderConfig(Logger logger, RenderConfigType type);
  ~OpenGLRenderConfig() final = default;

  //Getting rid of copy/move constructors/assignment operators (may need later)
  OpenGLRenderConfig(const OpenGLRenderConfig&) = delete;
  OpenGLRenderConfig& operator=(const OpenGLRenderConfig&) = delete;
  OpenGLRenderConfig(OpenGLRenderConfig&&) = delete;
  OpenGLRenderConfig& operator=(OpenGLRenderConfig&&) = delete;

  void SetCullFace(bool is_enabled) final { cull_face_ = is_enabled; };
  void SetDepthTest(bool is_enabled) final { depth_test_ = is_enabled; };
  void SetScissorTest(bool is_enabled) final { scissor_test_ = is_enabled; };
  void SetBlend(int src_func, int dest_func, int equation) final;
  void UnsetBlend() final { blend_ = false; };
  void SetPolygonMode(int face_enum, int mode_enum) final {
    polygon_face_ = (GLenum)face_enum; polygon_mode_ = (GLenum)mode_enum;
  }; //TODO: Check the enums to see if they are valid

  void ApplyConfig() final;

 private:
  bool cull_face_;
  bool depth_test_;
  bool scissor_test_;

  bool blend_;
  GLenum blend_src_func_;
  GLenum blend_dest_func_;
  GLenum blend_equation_;

  GLenum polygon_face_;
  GLenum polygon_mode_;

  Logger logger_;
};

} // namespace quasi_game_engine

#endif //QGE_OPENGL_RENDER_CONFIG_H
