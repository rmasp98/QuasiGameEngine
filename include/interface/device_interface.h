/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/
#ifndef QGE_DEVICE_INTERFACE_H
#define QGE_DEVICE_INTERFACE_H

#include <memory>

// TODO: remove when sorted imgui stuff!
#include <GLFW/glfw3.h>

namespace quasi_game_engine {

//As input is not really used here, we will just forward declare it
class Input;

class DeviceInterface {
/*------------------------------------------------------------------------------
  Interface class for overall interaction with different window managers.
  Responsible for presenting a unified interface into any supported wm.
  All sub classes should have a window, a logger, and an input class
Notes
- Notes go here
------------------------------------------------------------------------------*/
 public:
  virtual ~DeviceInterface() = default;

  //Getting rid of copy/move constructors/assignment operators (may need later)
  DeviceInterface(const DeviceInterface&) = delete;
  DeviceInterface& operator=(const DeviceInterface&) = delete;
  DeviceInterface(DeviceInterface&&) = delete;
  DeviceInterface& operator=(DeviceInterface&&) = delete;

  virtual void Update() = 0;      // called every frame to update entire wm
  virtual void SwapBuffers() = 0; // called at end of each frame
  virtual void PollEvents() = 0;  // called at beginning of each frame to get user input
  virtual bool IsWindowOpen() const = 0;
  virtual const std::shared_ptr<Input> GetInput() const = 0;

 protected:
  DeviceInterface() = default;

};

}  // namespace quasi_game_engine

#endif // QGE_DEVICE_INTERFACE_H
