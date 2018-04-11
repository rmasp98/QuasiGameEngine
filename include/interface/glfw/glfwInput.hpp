#ifndef QGE_GLFW_INPUT_HPP
#define QGE_GLFW_INPUT_HPP

#include "interface/input.hpp"

#include <GLFW/glfw3.h>

#include <string>
#include <map>


namespace qge {
   class Logger;

   class GlfwInput : public Input {
      friend class GlfwInterface;
   public:
      ~GlfwInput();

   protected:
      GLFWwindow *window;

      GlfwInput(Logger *loggerIn, GLFWwindow *windowIn, const char *configFileNameIn);

      void update();
      bool isPressed(const Key keyIn);
      bool validKey(uint keyValueIn);

   };

} // namespave qge


#endif // QGE_GLFW_INPUT_HPP
