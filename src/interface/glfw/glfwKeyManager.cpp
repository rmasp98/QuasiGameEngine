#include "interface/glfw/glfwKeyManager.hpp"

#include "utils/logging/logger.hpp"

#include <math.h>


namespace xxx {

   GlfwKeyManager::GlfwKeyManager(Logger* loggerIn, GLFWwindow* windowIn)
                               : KeyManager(loggerIn), window(windowIn) {}


   GlfwKeyManager::~GlfwKeyManager() {

   }

   //This can probably be moved to the constructor
   bool GlfwKeyManager::init(const char* configFileNameIn) {

      //Load keys from file
      setInput = "keyboard"; //load from a config file
      loadKeys(configFileNameIn);

      //Can this go here? Currently in window loader
      glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE); //Allows key presses to be detected in frame
      //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //Hide cursor

      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

      // Preset the last mouse position to prevent jumping at the begining
      glfwGetCursorPos(window, &lastPos[0], &lastPos[1]);


      //May need to determine center of window

      return true;

   }


   bool GlfwKeyManager::validKey(uint keyValueIn) {

      //this is to check input is a valid key on the keyboard

      return true;

   }





   void GlfwKeyManager::update() {

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

      diffPos[0] = lastPos[0] - cursor[0]; diffPos[1] = lastPos[1] - cursor[1];

      //send active keys and cursor position out to everyone that needs it

   }




   bool GlfwKeyManager::isPressed(const Key keyIn) {

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
