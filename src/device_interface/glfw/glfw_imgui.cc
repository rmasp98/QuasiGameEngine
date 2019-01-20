/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/
#include "device_interface/glfw/glfw_imgui.h"

#include "utils/logging/log_capture.h"

namespace quasi_game_engine {

std::unordered_set<int> pressed_keys_;

GlfwImgui::GlfwImgui(GLFWwindow* window, ImGuiIO* io) : window_(window) {
  io->SetClipboardTextFn = SetClipboardText;
  io->GetClipboardTextFn = GetClipboardText;
  io->ClipboardUserData = window;
  // Keyboard mapping. ImGui will use those indices to peek into the
  // io.KeysDown[] array.
  io->KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
  io->KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
  io->KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
  io->KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
  io->KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
  io->KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
  io->KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
  io->KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
  io->KeyMap[ImGuiKey_End] = GLFW_KEY_END;
  io->KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
  io->KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
  io->KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
  io->KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
  io->KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
  io->KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
  io->KeyMap[ImGuiKey_A] = GLFW_KEY_A;
  io->KeyMap[ImGuiKey_C] = GLFW_KEY_C;
  io->KeyMap[ImGuiKey_V] = GLFW_KEY_V;
  io->KeyMap[ImGuiKey_X] = GLFW_KEY_X;
  io->KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
  io->KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

#ifdef _WIN32
  io->ImeWindowHandle = glfwGetWin32Window(window);
#endif
  // Load cursors
  // FIXME: GLFW doesn't expose suitable cursors for ResizeAll, ResizeNESW,
  // ResizeNWSE. We revert to arrow cursor for those.
  mouse_cursors_[ImGuiMouseCursor_Arrow] =
      glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
  mouse_cursors_[ImGuiMouseCursor_TextInput] =
      glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
  mouse_cursors_[ImGuiMouseCursor_ResizeAll] =
      glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
  mouse_cursors_[ImGuiMouseCursor_ResizeNS] =
      glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
  mouse_cursors_[ImGuiMouseCursor_ResizeEW] =
      glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
  mouse_cursors_[ImGuiMouseCursor_ResizeNESW] =
      glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
  mouse_cursors_[ImGuiMouseCursor_ResizeNWSE] =
      glfwCreateStandardCursor(GLFW_ARROW_CURSOR);

  glfwSetKeyCallback(window, KeyCallback);
  glfwSetCharCallback(window, CharCallback);
  glfwSetScrollCallback(window, ScrollCallback);
  glfwSetMouseButtonCallback(window, MouseButtonCallback);
}

GlfwImgui::~GlfwImgui() {
  // Destroy GLFW mouse cursors
  for (ImGuiMouseCursor cursor_n = 0; cursor_n < ImGuiMouseCursor_COUNT;
       cursor_n++)
    glfwDestroyCursor(mouse_cursors_[cursor_n]);
  memset(mouse_cursors_, 0, sizeof(mouse_cursors_));

  glfwSetKeyCallback(window_, nullptr);
  glfwSetCharCallback(window_, nullptr);
  glfwSetScrollCallback(window_, nullptr);
  glfwSetMouseButtonCallback(window_, nullptr);
}

std::unordered_set<int> GlfwImgui::GetPressedKeys() { return pressed_keys_; }

void GlfwImgui::Update(GLFWwindow* window) {
  // Setup display size (every frame to accommodate for window resizing)
  ImGuiIO& io = ImGui::GetIO();
  int w, h;
  int display_w, display_h;
  glfwGetWindowSize(window, &w, &h);
  glfwGetFramebufferSize(window, &display_w, &display_h);
  io.DisplaySize = ImVec2((float)w, (float)h);
  io.DisplayFramebufferScale = ImVec2(w > 0 ? ((float)display_w / w) : 0,
                                      h > 0 ? ((float)display_h / h) : 0);

  // Setup time step
  double current_time = glfwGetTime();
  io.DeltaTime =
      time_ > 0.0 ? (float)(current_time - time_) : (float)(1.0f / 60.0f);
  time_ = current_time;

  if ((io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) == 0 &&
      glfwGetInputMode(window, GLFW_CURSOR) != GLFW_CURSOR_DISABLED) {
    ImGuiMouseCursor cursor = ImGui::GetMouseCursor();
    if (io.MouseDrawCursor || cursor == ImGuiMouseCursor_None) {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    } else {
      glfwSetCursor(window, mouse_cursors_[cursor]
                                ? mouse_cursors_[cursor]
                                : mouse_cursors_[ImGuiMouseCursor_Arrow]);
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
  }

  // Setup inputs
  // (we already got mouse wheel, keyboard keys & characters from glfw callbacks
  // polled in glfwPollEvents())
  if (glfwGetWindowAttrib(window, GLFW_FOCUSED)) {
    // Set OS mouse position if requested (only used when
    // ImGuiConfigFlags_NavEnableSetMousePos is enabled by user)
    if (io.WantSetMousePos) {
      glfwSetCursorPos(window, (double)io.MousePos.x, (double)io.MousePos.y);
    } else {
      double mouse_x, mouse_y;
      glfwGetCursorPos(window, &mouse_x, &mouse_y);
      io.MousePos = ImVec2((float)mouse_x, (float)mouse_y);
    }
  } else {
    io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
  }
}

void GlfwImgui::KeyCallback(GLFWwindow* window, int key, int scancode,
                            int action, int mod) {
  if (action == GLFW_PRESS) {
    pressed_keys_.insert(key);
  } else if (action == GLFW_RELEASE) {
    pressed_keys_.erase(key);
  }

  // Temporary until we figure out how to get these values across
  ImGuiIO& io = ImGui::GetIO();
  if (action == GLFW_PRESS) io.KeysDown[key] = true;
  if (action == GLFW_RELEASE) io.KeysDown[key] = false;

  (void)mod;  // Modifiers are not reliable across systems
  io.KeyCtrl =
      io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
  io.KeyShift =
      io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
  io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
  io.KeySuper =
      io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
}

void GlfwImgui::CharCallback(GLFWwindow*, unsigned int c) {
  ImGuiIO& io = ImGui::GetIO();
  if (c > 0 && c < 0x10000) io.AddInputCharacter((unsigned short)c);
}

void GlfwImgui::ScrollCallback(GLFWwindow*, double xoffset, double yoffset) {
  ImGuiIO& io = ImGui::GetIO();
  io.MouseWheelH -= (float)xoffset;
  io.MouseWheel -= (float)yoffset;
}

void GlfwImgui::MouseButtonCallback(GLFWwindow*, int button, int action,
                                    int /*mods*/) {
  if (button >= 0 && button < 3) {
    ImGuiIO& io = ImGui::GetIO();
    if (action == GLFW_PRESS)
      io.MouseDown[button] = true;
    else if (action == GLFW_RELEASE)
      io.MouseDown[button] = false;
  }
}

const char* GlfwImgui::GetClipboardText(void* user_data) {
  return glfwGetClipboardString((GLFWwindow*)user_data);
}

void GlfwImgui::SetClipboardText(void* user_data, const char* text) {
  glfwSetClipboardString((GLFWwindow*)user_data, text);
}

}  // namespace quasi_game_engine
