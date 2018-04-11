#include "interface/glfw/glfwInterface.hpp"

#include "utils/logging/logger.hpp"
#include "utils/logging/logWorker.hpp"

#include <math.h>


namespace qge {

   GlfwInterface::GlfwInterface(LogWorker* logWorkerIn, const char* configFileNameIn)
                              : DeviceInterface(logWorkerIn) {

      LOG(LOG_INFO, logger) << "Initialising GLFW";
      if (!glfwInit()) {
         LOG(LOG_FATAL, logger) << "Failed to initialise GLFW";
      }

      logWorkerIn->setTimeFunction(&glfwGetTime);

      // Need to read this in from config file
      //glfwWindowHint(GLFW_SAMPLES, 4); //Antialiasing = 4 samples
      glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Pointing to version of openGL
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Should find out about other profiles

      //Enable mutlisampling Antialiasing
      // glfwWindowHint(GLFW_SAMPLES, 4);
      // glEnable(GL_MULTISAMPLE);

      //Gets information about the primary monitor
      GLFWmonitor* monitor = glfwGetPrimaryMonitor();
      const GLFWvidmode* mode = glfwGetVideoMode(monitor);
      glfwWindowHint(GLFW_RED_BITS, mode->redBits);
      glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
      glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
      glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

      LOG(LOG_INFO, logger) << "Creating GLFW window";

      //Creates the window
      //window = glfwCreateWindow(mode->width, mode->height, "First Program", NULL, NULL);
      //window = glfwCreateWindow(800, 600, "First Program", monitor, NULL);
      //window = glfwCreateWindow(800, 600, "First Program", NULL, NULL);
      window = glfwCreateWindow(1920, 1080, "First Program", monitor, NULL);
      //window = glfwCreateWindow(1680, 1050, "First Program", monitor, NULL);


      //This one sets up full screen but there is currently a bug that produces a blue strip
      //window = glfwCreateWindow(mode->width, mode->height, "First Program", mode, NULL);

      //Checks to ensure a window was created properly
      if (window == NULL) {
         LOG(LOG_FATAL, logger) << "Failed to open GLFW window";
      }

      glfwMakeContextCurrent(window); //Makes this window the current window


      input = new GlfwInput(logger, window, configFileNameIn);
   };


   GlfwInterface::~GlfwInterface() {
      glfwTerminate();
   }




   void GlfwInterface::swapBuffers() {
      glfwSwapBuffers(window);
   }



   void GlfwInterface::pollEvents() {
      glfwPollEvents();
   }



   bool GlfwInterface::isWindowOpen() {
      //This is tempoary and should be changed later
      return !input->isKeyActive(KEY_ESC) && glfwWindowShouldClose(window) == 0;
   }




   void GlfwInterface::update() {

      //May need to update window

      input->update();

   }



   void setApiVersion(short major, short minor, bool isCore) {

   }


}  // namespave qge
