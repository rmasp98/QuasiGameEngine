#ifndef QGE_DEVICE_INTERFACE_H
#define QGE_DEVICE_INTERFACE_H

#include "interface/input.h"

#include <memory>

#include <GLFW/glfw3.h>

namespace quasi_game_engine {

class DeviceInterface {
/*Notes-------------------------------------------------------------------------
- Notes go here
------------------------------------------------------------------------------*/
 public:
  virtual ~DeviceInterface() = default;

  //Getting rid of copy/move constructors/assignment operators (may need later)
  DeviceInterface(const DeviceInterface&) = delete;
  DeviceInterface& operator=(const DeviceInterface&) = delete;
  DeviceInterface(DeviceInterface&&) = delete;
  DeviceInterface& operator=(DeviceInterface&&) = delete;

  virtual void Update() = 0;
  virtual void SwapBuffers() = 0;
  virtual void PollEvents() = 0;
  virtual bool IsWindowOpen() const = 0;
  virtual const Input* GetInput() const = 0;

  //TODO: get rid of once imgui is set up
  virtual GLFWwindow *GetWindow() const = 0;

 protected:
  DeviceInterface() = default;

};

}  // namespace quasi_game_engine

#endif // QGE_DEVICE_INTERFACE_H
