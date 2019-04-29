#ifndef QGE_UI_TEST_H
#define QGE_UI_TEST_H

#include "device_interface/input.h"
#include "renderer/opengl/opengl_shader.h"
#include "renderer/renderer.h"
#include "utils/logging/log_capture.h"

#include <memory>
#include <string>
#include <vector>

namespace quasi_game_engine {

class UserInterface {
  /*---------------------------------------------------------------------------
  NOTES
  ---------------------------------------------------------------------------*/
 public:
  UserInterface(Renderer* render_manager, std::shared_ptr<Input> input);
  ~UserInterface();

  void Update(Renderer* render_manager);

 protected:
  std::vector<BufferIds> buffer_ids;
  std::shared_ptr<Input> input_;  // Kept to delete GlfwImgui when destroyed

  void LoadToGraphics(Renderer* render_manager, int buffer_height);
};

}  // namespace quasi_game_engine

#endif  // QGE_UI_TEST_H
