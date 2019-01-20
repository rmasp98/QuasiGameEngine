/*------------------------------------------------------------------------------
  Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
  All rights reserved.

  This software is licensed as described in the file LICENSE.md, which
  you should have received as part of this distribution.

  Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_GLFW_INPUT_H
#define QGE_GLFW_INPUT_H

#include "device_interface/action_manager.h"
#include "device_interface/glfw/glfw_imgui.h"
#include "device_interface/input.h"

#include <GLFW/glfw3.h>

#include "imgui/imgui.h"

namespace quasi_game_engine {

/*------------------------------------------------------------------------------
  GLFW implementation of the input interface. Apart from construction, it
  should never be used directly. This class should only be created and modified
  by GlfwInterface, only get functions can be used externally

  Notes
   - Should change update to a set of callbacks
------------------------------------------------------------------------------*/
class GlfwInput : public Input {
 public:
  GlfwInput(GLFWwindow* window, const char* config_file_name);
  ~GlfwInput() final = default;

  // Getting rid of copy/move constructors/assignment operators (may need later)
  GlfwInput(const GlfwInput&) = delete;
  GlfwInput& operator=(const GlfwInput&) = delete;
  GlfwInput(GlfwInput&&) = delete;
  GlfwInput& operator=(GlfwInput&&) = delete;

  // This is static to allow action list to be able to call it
  static bool ValidButton(int button_value);

  void Update() final;
  const double* GetMouseMovement() const final { return diff_pos_; };
  const double* GetMousePosition() const final { return cursor_; };
  void InitGuiIO(ImGuiIO* io) final { imgui_io_ = new GlfwImgui(window_, io); }
  void DestroyGuiIO() final;

 private:
  ActionManager action_manager;
  GLFWwindow* window_;  // This has to be a pointer and is managed by GLFW
  double cursor_[2], diff_pos_[2];  // This only double because of stupid glfw!
  GlfwImgui* imgui_io_;
};

}  // namespace quasi_game_engine

#endif  // QGE_GLFW_INPUT_H
