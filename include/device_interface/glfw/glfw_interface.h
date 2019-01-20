/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_GLFW_INTERFACE_H
#define QGE_GLFW_INTERFACE_H

#include "device_interface/device_interface.h"
#include "device_interface/glfw/glfw_input.h"

#include <GLFW/glfw3.h>

#include <memory>

namespace quasi_game_engine {

/*------------------------------------------------------------------------------
Glfw implementation of the device interface. Apart from construction, it
should never be used directly. This class will initialise GLFW, act as a
wrapper for key functions and update the input class
Notes:
  - Will need to tidy up constructor when I understand other interfaces better
  - need to implement some of the functionality of window if we decide to use
  more than one:
  - set to current window
  - set size, position, title, anti-aliasing (this should be in a setting
  struct)
  - need to read in config from file
  - Might need to split option settings in APIs but unlikely
  - Need to think about an initial pop up window to define final window settings
------------------------------------------------------------------------------*/
class GlfwInterface : public DeviceInterface {
 public:
  explicit GlfwInterface(const char* config_file_name);
  ~GlfwInterface() { glfwTerminate(); };

  // Getting rid of copy/move constructors/assignment operators (may need later)
  GlfwInterface(const GlfwInterface&) = delete;
  GlfwInterface& operator=(const GlfwInterface&) = delete;
  GlfwInterface(GlfwInterface&&) = delete;
  GlfwInterface& operator=(GlfwInterface&&) = delete;

 protected:
  void Update() final;
  void SwapBuffers() final;
  void PollEvents() final;
  bool IsWindowOpen() const final;
  const std::shared_ptr<Input> GetInput() const final { return input_; };

 private:
  std::shared_ptr<GlfwInput> input_;
  // GLFW requires this being a pointer and is managed by GLFW
  GLFWwindow* window_;

  void CreateWindow(const char* title, int width, int height, bool full_screen);
};

}  // namespace quasi_game_engine

#endif  // QGE_GLFW_INTERFACE_H
