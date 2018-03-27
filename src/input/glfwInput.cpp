#include "input/glfwInput.hpp"

#include "utils/logging/logger.hpp"
#include "utils/logging/logWorker.hpp"

#include <math.h>


namespace xxx {

   GLFWInput::GLFWInput(LogWorker* logWorkerIn)  : Input(logWorkerIn) {
      logWorkerIn->setTimeFunction(&glfwGetTime);
   };


   GLFWInput::~GLFWInput() {
      glfwTerminate();
   }


   bool GLFWInput::init(const char* configFileNameIn) {


      LOG(LOG_INFO, logger) << "Initialising GLFW";
      if (!glfwInit()) {
         LOG(LOG_FATAL, logger) << "Failed to initialise GLFW";
         return false;
      }


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


      //Do properly!
      winCen[0] = 1920.0/2.0;
      winCen[1] = 1080.0/2.0;


      //This one sets up full screen but there is currently a bug that produces a blue strip
      //window = glfwCreateWindow(mode->width, mode->height, "First Program", mode, NULL);

      //Checks to ensure a window was created properly
      if (window == NULL) {
          LOG(LOG_FATAL, logger) << "Failed to open GLFW window";
          return false;
      }

      glfwMakeContextCurrent(window); //Makes this window the current window

      glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE); //Allows key presses to be detected in frame
      //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //Hide cursor

      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

      // Preset the last mouse position to prevent jumping at the begining
      glfwGetCursorPos(window, &lastPos[0], &lastPos[1]);
      //glfwSetCursorPosCallback(window, mouseCallback);


      //Load keys from file
      setInput = "keyboard"; //load from a config file
      loadKeys(configFileNameIn);

      return true;

   }


   void GLFWInput::swapBuffers() {
      glfwSwapBuffers(window);
   }



   void GLFWInput::pollEvents() {
      glfwPollEvents();
   }

   bool GLFWInput::isWindowOpen() {
      return glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS
               && glfwWindowShouldClose(window) == 0;
   }


   bool GLFWInput::validKey(uint keyValueIn) {

      //this is to check input is a valid key on the keyboard

      return true;

   }





   void GLFWInput::update() {

      keyState = 0;
      GLuint it = 0;
      for (auto const &ent1 : keys) {
         if (isPressed(ent1.second))
            keyState += pow(2,it);
         it++;
      }

      // key state changed if keyState xor (not the same as) oldKeyState
      uint stateChange = keyState ^ oldKeyState;
      // active keys if the key is a hold key or has changed and is pressed down
      activeKeys = (holdKeys | stateChange) & keyState;
      oldKeyState = keyState;

      //get cursor position
      lastPos[0] = cursor[0]; lastPos[1] = cursor[1];
      glfwGetCursorPos(window, &cursor[0], &cursor[1]);

      //send active keys and cursor position out to everyone that needs it

   }




   bool GLFWInput::isPressed(const Key keyIn) {

      if (true) { //Is key
         if (glfwGetKey(window, keyIn.keyValue) == GLFW_PRESS)
            return true;
      } else if (true) { //Is mouse
         if (glfwGetMouseButton(window, keyIn.keyValue) == GLFW_PRESS)
            return true;
      }

      return false;
   }

}
