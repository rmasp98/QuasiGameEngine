#ifndef QGE_DEVICE_INTERFACE_H
#define QGE_DEVICE_INTERFACE_H

#include "interface/input.h"

#include <memory>

#include <GLFW/glfw3.h>

namespace quasi_game_engine {

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
  virtual void PollEvents() = 0;  // called at beginning of each frame
  virtual bool IsWindowOpen() const = 0;
  virtual const Input* GetInput() const = 0;

  //TODO: get rid of once imgui is set up
  virtual GLFWwindow *GetWindow() const = 0;

 protected:
  DeviceInterface() = default;

};

}  // namespace quasi_game_engine

#endif // QGE_DEVICE_INTERFACE_H
