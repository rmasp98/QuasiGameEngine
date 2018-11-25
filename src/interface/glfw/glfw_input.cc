/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#include "interface/glfw/glfw_input.h"

#include "utils/logging/log_capture.h"

#include <cmath>

namespace quasi_game_engine {

GlfwInput::GlfwInput(GLFWwindow* window, const char* config_file_name)
                     : window_(window) {
  //Load actions from file
  action_list_.LoadActionMapping(config_file_name);
  glfwSetInputMode(window_, GLFW_STICKY_KEYS, GL_TRUE); //Allows button presses to be detected in frame
  //TODO: this should probably be a public function
  //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //Hide cursor

  // Preset the last mouse position to prevent jumping at the beginning
  glfwGetCursorPos(window_, &cursor_[0], &cursor_[1]);
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

  // Populate cursor movement
  double last_pos[2] = { cursor_[0], cursor_[1] };
  glfwGetCursorPos(window_, &cursor_[0], &cursor_[1]);
  diff_pos_[0] = last_pos[0] - cursor_[0];
  diff_pos_[1] = last_pos[1] - cursor_[1];

  //TODO: send active buttons and cursor position out to everyone that needs it?
}


void GlfwInput::key_callback(GLFWwindow* window, int key, int scanmode, int action, int mode) {
  if (action == GLFW_PRESS) {}
    //pressed_keys[num_pressed_keys++] = key;
}



// This will go if we swtich to button callback
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
      LOG(WARN, INTERFACE) << "Action: '" 
          << action.GetName() << "' has not been set!";
      return false;
    }
  }
  return false;
}


//TODO: need to write this!
bool GlfwInput::ValidButton(int button_value) {
  //this is to check input is a valid button on the keyboard
  if (button_value)
    return true;
  return false;
}

} // namespace quasi_game_engine
