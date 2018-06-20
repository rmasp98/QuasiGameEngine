#ifndef QGE_GLFW_INPUT_H
#define QGE_GLFW_INPUT_H

#include "interface/input.h"
#include "interface/action_list.h"

#include <GLFW/glfw3.h>

namespace quasi_game_engine {
class Logger;

class GlfwInput : public Input {
/*Notes-------------------------------------------------------------------------
- Notes go here
------------------------------------------------------------------------------*/
  friend class GlfwInputHelper;
 public:
  ~GlfwInput() = default;

  //Getting rid of copy/move constructors/assignment operators (may need later)
  GlfwInput(const GlfwInput&) = delete;
  GlfwInput& operator=(const GlfwInput&) = delete;
  GlfwInput(GlfwInput&&) = delete;
  GlfwInput& operator=(GlfwInput&&) = delete;

 protected:
  GlfwInput(Logger logger, GLFWwindow* window, const char* config_file_name);

  void Update() final;
  bool IsActionActive(const ActionEnum action) const final;
  const float* GetMouseMovement() const final { return diff_pos_; };
  const ActionList* GetActionList() const final { return &action_list_; };

 private:
  bool IsPressed(const Action action) const;
  bool ValidButton(const int button_value) const;

  Logger logger_;
  GLFWwindow* window_; // This has to be a pointer and is managed by GLFW
  ActionList action_list_;
  float cursor_[2], diff_pos_[2]; // possibly move this?
};

class GlfwInputHelper {
  friend class GlfwInterface;
  static GlfwInput* CreateGlfwInput(Logger logger, GLFWwindow* window,
                                    const char* config_file_name) {
    return new GlfwInput(logger, window, config_file_name);
  };

  static void DeleteInput(Input* input) { delete input; };
};

} // namespace quasi_game_engine

#endif // QGE_GLFW_INPUT_H
