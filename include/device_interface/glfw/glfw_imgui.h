/*------------------------------------------------------------------------------
  Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
  All rights reserved.

  This software is licensed as described in the file LICENSE.md, which
  you should have received as part of this distribution.

  Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_GLFW_IMGUI_H
#define QGE_GLFW_IMGUI_H

#include <GLFW/glfw3.h>

#include "imgui/imgui.h"

#include <unordered_set>

namespace quasi_game_engine {

class GlfwImgui {
 public:
  GlfwImgui(GLFWwindow* window, ImGuiIO* io);
  ~GlfwImgui();

  void Update(GLFWwindow* window);

  std::unordered_set<int> GetPressedKeys();

  static void KeyCallback(GLFWwindow* window, int key, int scancode, int action,
                          int mod);
  static void CharCallback(GLFWwindow*, unsigned int c);
  static void ScrollCallback(GLFWwindow*, double xoffset, double yoffset);
  static void MouseButtonCallback(GLFWwindow*, int button, int action,
                                  int /*mods*/);
  static const char* GetClipboardText(void* user_data);
  static void SetClipboardText(void* user_data, const char* text);

 private:
  GLFWcursor* mouse_cursors_[ImGuiMouseCursor_COUNT] = {0};
  double time_ = 0.0f;
  GLFWwindow* window_;  // Required to remove callbacks?
};

}  // namespace quasi_game_engine

#endif  // QGE_GLFW_IMGUI_H
