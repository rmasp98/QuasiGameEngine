#ifndef QGE_UI_TEST_H
#define QGE_UI_TEST_H

#include "renderer/renderer.h"
#include "renderer/render_config.h"
#include "renderer/opengl/opengl_shader.h"
#include "renderer/opengl/opengl_render_config.h"
#include "interface/input.h"
#include "utils/logging/log_capture.h"

#include <memory>
#include <vector>
#include <string>


namespace quasi_game_engine {
class Renderer;

class UserInterface {
/* NOTES --------------------------------------------------------------------

---------------------------------------------------------------------------*/
public:
  UserInterface(Renderer* render_manager);
  ~UserInterface();

  void Update(std::shared_ptr<Input> input, Renderer* render_manager);
  
protected:
  RenderConfig* uiRenderConfig;
  Shader* shader;
  int vao_id = 0, vbo_id = 0, index_id = 0;
};

} // namespace quasi_game_engine


#endif // QGE_UI_TEST_H
