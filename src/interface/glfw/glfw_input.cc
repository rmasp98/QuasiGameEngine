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
#include <unordered_set>

namespace quasi_game_engine {

std::unordered_set<int> pressed_keys;
static GLFWcursor*  g_MouseCursors[ImGuiMouseCursor_COUNT] = { 0 };
static double       g_Time = 0.0f;

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mod) {
  if (action == GLFW_PRESS) {
    pressed_keys.insert(key);
  } else if (action == GLFW_RELEASE) {
    pressed_keys.erase(key);
  }

  //Temporary until we figure out how to get these values across
  ImGuiIO& io = ImGui::GetIO();
  if (action == GLFW_PRESS)
    io.KeysDown[key] = true;
  if (action == GLFW_RELEASE)
    io.KeysDown[key] = false;

  (void)mod; // Modifiers are not reliable across systems
  io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
  io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
  io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
  io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
}

static void CharCallback(GLFWwindow*, unsigned int c) {
  ImGuiIO& io = ImGui::GetIO();
  if (c > 0 && c < 0x10000)
    io.AddInputCharacter((unsigned short)c);
}

//Need to find a way to match the OS preference for directtion
static void ScrollCallback(GLFWwindow*, double xoffset, double yoffset) {
  ImGuiIO& io = ImGui::GetIO();
  io.MouseWheelH -= (float)xoffset;
  io.MouseWheel -= (float)yoffset;
}

static void MouseButtonCallback(GLFWwindow*, int button, int action, int /*mods*/) {
  if (button >= 0 && button < 3) {
    ImGuiIO& io = ImGui::GetIO();
    if (action == GLFW_PRESS)
      io.MouseDown[button] = true;
    else if (action == GLFW_RELEASE)
      io.MouseDown[button] = false;
  }
}

static const char* GetClipboardText(void* user_data)
{
    return glfwGetClipboardString((GLFWwindow*)user_data);
}

static void SetClipboardText(void* user_data, const char* text)
{
    glfwSetClipboardString((GLFWwindow*)user_data, text);
}


GlfwInput::GlfwInput(GLFWwindow* window, const char* config_file_name)
                     : window_(window), action_manager(config_file_name) {
  //Load actions from file
  //action_list_.LoadActionMapping(config_file_name);
  //glfwSetInputMode(window_, GLFW_STICKY_KEYS, GL_TRUE); //Allows button presses to be detected in frame
  //TODO: this should probably be a public function
  //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //Hide cursor

  // Preset the last mouse position to prevent jumping at the beginning
  glfwGetCursorPos(window_, &cursor_[0], &cursor_[1]);

  glfwSetKeyCallback(window_, KeyCallback);
  glfwSetCharCallback(window_, CharCallback);
  glfwSetScrollCallback(window_, ScrollCallback); 
  glfwSetMouseButtonCallback(window_, MouseButtonCallback);

  // Load cursors
   // FIXME: GLFW doesn't expose suitable cursors for ResizeAll, ResizeNESW, ResizeNWSE. We revert to arrow cursor for those.
   g_MouseCursors[ImGuiMouseCursor_Arrow] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
   g_MouseCursors[ImGuiMouseCursor_TextInput] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
   g_MouseCursors[ImGuiMouseCursor_ResizeAll] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
   g_MouseCursors[ImGuiMouseCursor_ResizeNS] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
   g_MouseCursors[ImGuiMouseCursor_ResizeEW] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
   g_MouseCursors[ImGuiMouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
   g_MouseCursors[ImGuiMouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
}

GlfwInput::~GlfwInput() {
  // Destroy GLFW mouse cursors
  for (ImGuiMouseCursor cursor_n = 0; cursor_n < ImGuiMouseCursor_COUNT; cursor_n++)
    glfwDestroyCursor(g_MouseCursors[cursor_n]);
  memset(g_MouseCursors, 0, sizeof(g_MouseCursors));
}


void GlfwInput::Update() {
  action_manager.Update(pressed_keys);

  // Populate cursor movement
  double last_pos[2] = { cursor_[0], cursor_[1] };
  glfwGetCursorPos(window_, &cursor_[0], &cursor_[1]);
  diff_pos_[0] = last_pos[0] - cursor_[0];
  diff_pos_[1] = last_pos[1] - cursor_[1];

  //TODO: send active buttons and cursor position out to everyone that needs it?

}


//TODO: need to write this!
bool GlfwInput::ValidButton(int button_value) {
  //this is to check input is a valid button on the keyboard
  if (button_value)
    return true;
  return false;
}


void GlfwInput::InitGuiIO(ImGuiIO& io) {
  io.SetClipboardTextFn = SetClipboardText;
  io.GetClipboardTextFn = GetClipboardText;
  io.ClipboardUserData = window_;
  // Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
  io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
  io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
  io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
  io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
  io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
  io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
  io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
  io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
  io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
  io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
  io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
  io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
  io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
  io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
  io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
  io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
  io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
  io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
  io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
  io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
  io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

#ifdef _WIN32
   io.ImeWindowHandle = glfwGetWin32Window(g_Window);
#endif
}


void GlfwInput::UpdateGuiIO(ImGuiIO* io) {
  // Setup display size (every frame to accommodate for window resizing)
  int w, h;
  int display_w, display_h;
  glfwGetWindowSize(window_, &w, &h);
  glfwGetFramebufferSize(window_, &display_w, &display_h);
  io->DisplaySize = ImVec2((float)w, (float)h);
  io->DisplayFramebufferScale = ImVec2(w > 0 ? ((float)display_w / w) : 0, h > 0 ? ((float)display_h / h) : 0);

  // Setup time step
  double current_time =  glfwGetTime();
  io->DeltaTime = g_Time > 0.0 ? (float)(current_time - g_Time) : (float)(1.0f/60.0f);
  g_Time = current_time;

  if ((io->ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) == 0 && glfwGetInputMode(window_, GLFW_CURSOR) != GLFW_CURSOR_DISABLED) {
    ImGuiMouseCursor cursor = ImGui::GetMouseCursor();
    if (io->MouseDrawCursor || cursor == ImGuiMouseCursor_None) {
      glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    } else {
      glfwSetCursor(window_, g_MouseCursors[cursor] ? g_MouseCursors[cursor] : g_MouseCursors[ImGuiMouseCursor_Arrow]);
      glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
  }

  // Setup inputs
  // (we already got mouse wheel, keyboard keys & characters from glfw callbacks polled in glfwPollEvents())
  if (glfwGetWindowAttrib(window_, GLFW_FOCUSED)) {
      // Set OS mouse position if requested (only used when ImGuiConfigFlags_NavEnableSetMousePos is enabled by user)
      if (io->WantSetMousePos) {
          glfwSetCursorPos(window_, (double)io->MousePos.x, (double)io->MousePos.y);
      } else {
          double mouse_x, mouse_y;
          glfwGetCursorPos(window_, &mouse_x, &mouse_y);
          io->MousePos = ImVec2((float)mouse_x, (float)mouse_y);
      }
  } else {
      io->MousePos = ImVec2(-FLT_MAX,-FLT_MAX);
  }
}

} // namespace quasi_game_engine
