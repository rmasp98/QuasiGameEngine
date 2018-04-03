#ifndef XXX_GLFW_INTERFACE_HPP
#define XXX_GLFW_INTERFACE_HPP

#include "interface/deviceInterface.hpp"
#include "interface/glfw/glfwKeyManager.hpp"
#include "utils/jsonFileManager/jsonFileManager.hpp"

#include <GLFW/glfw3.h>

#include <string>
#include <map>


namespace xxx {
   class LogWorker;

   class GlfwInterface : public DeviceInterface {
   public:
      GlfwInterface(LogWorker* logWorkerIn);
      ~GlfwInterface();
      bool init(const char* configFileNameIn);

   protected:
      GLFWwindow* window;

      KeyManager* keyMan;

      void swapBuffers();
      void pollEvents();
      bool isWindowOpen();

      bool isKeyActive(ActionEnum action);
      float* getMousePosDiff();

      void update();
   };

}


#endif // XXX_GLFW_INTERFACE_HPP
