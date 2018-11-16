/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#include "renderer/opengl/opengl_render_config.h"

namespace quasi_game_engine {

OpenGLRenderConfig::OpenGLRenderConfig(Logger logger, RenderConfigType type)
    : logger_(logger) {
  //TODO: read this from a file?
  if (type == RC_DEFAULT) {
    cull_face_ = true; depth_test_ = true; scissor_test_ = false;
    UnsetBlend();
    SetPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  } else if (type == RC_IMGUI) {
    cull_face_ = false; depth_test_ = false; scissor_test_ = true;
    SetBlend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_FUNC_ADD);
    SetPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  } else { // Initialise the config to ensure it works if not set
    cull_face_ = false; depth_test_ = false; scissor_test_ = false;
    UnsetBlend();
    SetPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    LOG(LOG_WARN, &logger_) << "Render Config has not been set so has been"
                           << " initialised all settings to off.";
  }
}


//TODO: check the enums to see if valid
void OpenGLRenderConfig::SetBlend(int src_func, int dest_func, int equation) {
  blend_ = true;
  blend_src_func_ = (GLenum)src_func;
  blend_dest_func_ = (GLenum)dest_func;
  blend_equation_ = (GLenum)equation;
}


//TODO: There has to be a better way of doing this without so many ifs
void OpenGLRenderConfig::ApplyConfig() {
  if (cull_face_)
    glEnable(GL_CULL_FACE);
  else
    glDisable(GL_CULL_FACE);

  if (depth_test_)
    glEnable(GL_DEPTH_TEST);
  else
    glDisable(GL_DEPTH_TEST);

  if (scissor_test_)
    glEnable(GL_SCISSOR_TEST);
  else
    glDisable(GL_SCISSOR_TEST);

  if (blend_) {
    glEnable(GL_BLEND);
    glBlendEquation(blend_equation_);
    glBlendFunc(blend_src_func_, blend_dest_func_);
  } else {
    glDisable(GL_BLEND);
  }

  // This may need full functionality
  glPolygonMode(polygon_face_, polygon_mode_);
}


} // namespace quasi_game_engine
