#ifndef XXX_GLFW_INPUT_HPP
#define XXX_GLFW_INPUT_HPP

#include "input/input.hpp"
#include "utils/jsonFileManager/jsonFileManager.hpp"

#include <GLFW/glfw3.h>

#include <string>
#include <map>


namespace xxx {
   class LogWorker;

   class GLFWInput : public Input {
   public:
      GLFWInput(LogWorker* logWorkerIn);

      ~GLFWInput();
      bool init(const char* configFileNameIn);

      void swapBuffers();
      void pollEvents();
      bool isWindowOpen();

      void update();

   protected:
      bool validKey(uint keyValueIn);
      GLFWwindow* window;

      bool isPressed(const Key keyIn);

   };

}


#endif // XXX_GLFW_INPUT_HPP
