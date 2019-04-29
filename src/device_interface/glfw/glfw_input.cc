/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#include "device_interface/glfw/glfw_input.h"
#include "device_interface/glfw/glfw_imgui.h"

#include "utils/logging/log_capture.h"

#include <cmath>
#include <unordered_set>

namespace quasi_game_engine {

std::unordered_set<int> pressed_keys;
void KeyCallback(GLFWwindow*, int key, int, int action, int) {
  if (action == GLFW_PRESS) {
    pressed_keys.insert(key);
  } else if (action == GLFW_RELEASE) {
    pressed_keys.erase(key);
  }
}

GlfwInput::GlfwInput(GLFWwindow* window, const char* config_file_name)
    : window_(window), action_manager(config_file_name) {
  // glfwSetInputMode(window_, GLFW_STICKY_KEYS, GL_TRUE); //Allows button
  // presses to be detected in frame
  // TODO: this should probably be a public function
  // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //Hide
  // cursor

  // Preset the last mouse position to prevent jumping at the beginning
  glfwGetCursorPos(window_, &cursor_[0], &cursor_[1]);

  glfwSetKeyCallback(window_, KeyCallback);
}

void GlfwInput::Update() {
  if (imgui_io_) {
    imgui_io_->Update(window_);
    action_manager.Update(imgui_io_->GetPressedKeys());
  } else {
    action_manager.Update(pressed_keys);
  }

  // Populate cursor movement
  double last_pos[2] = {cursor_[0], cursor_[1]};
  glfwGetCursorPos(window_, &cursor_[0], &cursor_[1]);
  diff_pos_[0] = last_pos[0] - cursor_[0];
  diff_pos_[1] = last_pos[1] - cursor_[1];

  // TODO: send active buttons and cursor position out to everyone that needs
  // it?
}

void GlfwInput::DestroyGuiIO() {
  if (imgui_io_) {
    delete imgui_io_;
  }

  glfwSetKeyCallback(window_, KeyCallback);
}

// TODO: need to write this!
bool GlfwInput::ValidButton(int button_value) {
  // this is to check input is a valid button on the keyboard
  if (button_value) return true;
  return false;
}

}  // namespace quasi_game_engine
