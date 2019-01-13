#ifndef QGE_UI_TEST_H
#define QGE_UI_TEST_H

#include "interface/input.h"
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
  UserInterface(Renderer* render_manager);
  ~UserInterface();

  void Update(std::shared_ptr<Input> input, Renderer* render_manager);

 protected:
  std::vector<BufferIds> buffer_ids;

  void LoadToGraphics(Renderer* render_manager, int buffer_height);
};

}  // namespace quasi_game_engine

#endif  // QGE_UI_TEST_H
