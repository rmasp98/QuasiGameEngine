#ifndef QGE_GLFW_INTERFACE_H
#define QGE_GLFW_INTERFACE_H

#include "interface/device_interface.h"
#include "interface/glfw/glfw_input.h"

#include <memory>

#include <GLFW/glfw3.h>

namespace quasi_game_engine {
class LogWorker;

class GlfwInterface : public DeviceInterface {
/* NOTES --------------------------------------------------------------------
- Will need to tidy up constructor when I understand other interfaces better
- need to implement so of the functionality of window if we decide to use more than one:
- set to current window
- set size, position, title, anti-aliasing
- need to read in config from file
- Might need to split option settings in APIs but unlikely
- Need to think about an initial pop up window to define final window settings
---------------------------------------------------------------------------*/
 public:
  GlfwInterface(LogWorker* log_worker, const char* config_file_name);
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
  const Input* GetInput() const final { return input_; };

 private:
  Logger logger_;
  Input* input_;
  GLFWwindow* window_;

  void CreateWindow(const char* name, int width, int height, bool full_screen);
};

} // namespace quasi_game_engine

#endif // QGE_GLFW_INTERFACE_H
