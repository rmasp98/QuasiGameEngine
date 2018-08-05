#ifndef QGE_GLFW_RENDER_CONFIG_H
#define QGE_GLFW_RENDER_CONFIG_H

#include "renderer/render_config.h"

#include "utils/logging/logger.h"

#include <GL/glew.h>

namespace quasi_game_engine {

class GlfwRenderConfig : public RenderConfig {
/*Notes-------------------------------------------------------------------------
- Notes go here
------------------------------------------------------------------------------*/
 public:
  GlfwRenderConfig(Logger logger, RenderConfigType type);
  ~GlfwRenderConfig() final = default;

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

#endif //QGE_GLFW_RENDER_CONFIG_H
