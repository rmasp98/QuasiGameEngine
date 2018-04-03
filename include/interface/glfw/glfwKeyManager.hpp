#ifndef XXX_GLFW_KEYS_HPP
#define XXX_GLFW_KEYS_HPP

#include "interface/keyManager.hpp"
#include "utils/jsonFileManager/jsonFileManager.hpp"

#include <GLFW/glfw3.h>

#include <string>
#include <map>


namespace xxx {
   class Logger;

   class GlfwKeyManager : public KeyManager {
      friend class GlfwInterface;
   public:
      ~GlfwKeyManager();
      bool init(const char* configFileNameIn);

      void update();

   protected:
      GLFWwindow* window;
      const char* setInput;

      GlfwKeyManager(Logger* loggerIn, GLFWwindow* windowIn);

      bool isPressed(const Key keyIn);
      bool validKey(uint keyValueIn);

   };

}


#endif // XXX_GLFW_KEYS_HPP
