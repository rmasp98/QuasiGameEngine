#include "interface/glfw/glfw_input.h"

#include "utils/logging/logger.h"

#include <cmath>

namespace quasi_game_engine {

GlfwInput::GlfwInput(Logger logger, GLFWwindow* window,
                     const char* config_file_name)
                     : logger_(logger), window_(window), action_list_(logger_) {

  //Load actions from file
  action_list_.LoadActionMapping(config_file_name);
  glfwSetInputMode(window_, GLFW_STICKY_KEYS, GL_TRUE); //Allows button presses to be detected in frame
  //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //Hide cursor

  // Preset the last mouse position to prevent jumping at the beginning
  glfwGetCursorPos(window_, &cursor_[0], &cursor_[1]);

}


bool GlfwInput::ValidButton(const int button_value) const {

  //this is to check input is a valid button on the keyboard

  return true;
}


bool GlfwInput::IsActionActive(const ActionEnum action) const {
  return action_list_.IsActionActive(action);
}


void GlfwInput::Update() {
  //Maybe change this to a button callback?
  int action_state = 0;
  int it = 0;
  for (auto const &action : action_list_.GetActions()) {
    if (IsPressed(action.second))
      action_state += pow(2,it);

    it++;
  }

  action_list_.Update(action_state);

  //get cursor position
  double last_pos[2] = {cursor_[0], cursor_[1]};
  glfwGetCursorPos(window_, &cursor_[0], &cursor_[1]);
  diff_pos_[0] = last_pos[0] - cursor_[0];
  diff_pos_[1] = last_pos[1] - cursor_[1];

  //send active buttons and cursor position out to everyone that needs it

}


bool GlfwInput::IsPressed(const Action action) const {
  for (int i_button = 0; i_button < action.GetNumButtons(); ++i_button) {
    Button button = action.GetButtons(i_button);
    if (button.button_type == BUTTON_KEYBOARD) { //Is key
      if (glfwGetKey(window_, button.button_value) == GLFW_PRESS)
        return true;
    } else if (button.button_type == BUTTON_MOUSE) { //Is mouse
      if (glfwGetMouseButton(window_, button.button_value) == GLFW_PRESS)
        return true;
    } else if (button.button_type == BUTTON_NULL) {
      LOG(LOG_WARN, &logger_) << "Action: '" << action.GetName()
                                                   << "' has not been set!";
      return false;
    }
  }
  return false;
}

} // namespace quasi_game_engine
