#ifndef QGE_GLFW_INTERFACE_HPP
#define QGE_GLFW_INTERFACE_HPP

#include "interface/deviceInterface.hpp"
#include "interface/glfw/glfwInput.hpp"

#include <GLFW/glfw3.h>

#include <string>
#include <map>


namespace qge {
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
      GlfwInterface(LogWorker *logWorkerIn, const char *configFileNameIn);
      ~GlfwInterface();


   protected:
      GLFWwindow *window;

      double getTime() { return glfwGetTime(); };
      void swapBuffers();
      void pollEvents();
      bool isWindowOpen();

      void update();

   };

} // namespave qge


#endif // QGE_GLFW_INTERFACE_HPP
