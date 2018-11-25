/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_GLFW_INTERFACE_H
#define QGE_GLFW_INTERFACE_H

#include "interface/device_interface.h"
#include "interface/glfw/glfw_input.h"

#include <memory>

#include <GLFW/glfw3.h>

namespace quasi_game_engine {
class LogWorker;

class GlfwInterface : public DeviceInterface {
/*------------------------------------------------------------------------------
  Glfw implementation of the device interface. Apart from construction, it should
  never be used directly. This class will initialise GLFW, act as a wrapper for
  key functions and update the input class
Notes:
- Will need to tidy up constructor when I understand other interfaces better
- need to implement some of the functionality of window if we decide to use more than one:
  - set to current window
  - set size, position, title, anti-aliasing (this should be in a setting struct)
  - need to read in config from file
- Might need to split option settings in APIs but unlikely
- Need to think about an initial pop up window to define final window settings
------------------------------------------------------------------------------*/
 public:
  explicit GlfwInterface(const char* config_file_name);
  ~GlfwInterface();

  //Getting rid of copy/move constructors/assignment operators (may need later)
  GlfwInterface(const GlfwInterface&) = delete;
  GlfwInterface& operator=(const GlfwInterface&) = delete;
  GlfwInterface(GlfwInterface&&) = delete;
  GlfwInterface& operator=(GlfwInterface&&) = delete;

  //TODO: get rid of once imgui is set up
  GLFWwindow* GetWindow() const final { return window_; };

 protected:
  void Update() final;
  void SwapBuffers() final;
  void PollEvents() final;
  bool IsWindowOpen() const final;
  // Input can only be deleted by this class (no need to smart pointers)
  const Input* GetInput() const final { return input_; };

 private:
  GlfwInput* input_;       // This is a pointer to a GlfwInput class
  GLFWwindow* window_; // GLFW requires this being a pointer and is managed by GLFW

  void CreateWindow(const char* title, int width, int height, bool full_screen);
};

} // namespace quasi_game_engine

#endif // QGE_GLFW_INTERFACE_H
