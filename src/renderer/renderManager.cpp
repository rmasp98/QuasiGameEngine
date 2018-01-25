#include "renderer/renderManager.hpp"
#include "resource/texture.hpp"

#include <iostream>


namespace xxx {

   RenderManager::RenderManager() {

   }


   RenderManager::~RenderManager() {

   }


   void RenderManager::draw() {

   }



   int RenderManager::initGraphics() {

      if (!glfwInit()) {
         std::cerr << "Failed to initialise GLFW\n";
         return -1;
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
          std::cerr << "Failed to open GLFW window\n";
          return -1;
      }
      glfwMakeContextCurrent(window); //Makes this window the current window


      glewExperimental = true; //Find out what this means
      const GLenum glewErr = glewInit();
      if (glewErr != GLEW_OK) {
          std::cerr << "Failed to initialise GLEW. "
                    << glewGetErrorString(glewErr) << std::endl;
          return -1;
      }

      glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE); //Allows key presses to be detected in frame
      //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //Hide cursor

      glClearColor(0.0, 0.0, 0.4, 0.0);   //Set default backgrond to dark blue
      glEnable(GL_DEPTH_TEST);            //Enable depth testing of objects
      glDepthFunc(GL_LESS);               //Accept if fragment is closer
      //glEnable(GL_CULL_FACE);             //Don't draw backfacing triangles#

      glEnable(GL_BLEND);
   	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



      return 0;
   }





   void RenderManager::loadTexture(Texture& texture) {

      uint textureID;
      glGenTextures(1, &textureID);

   	// "Bind" the newly created texture : all future texture functions will modify this texture
   	glBindTexture(GL_TEXTURE_2D, textureID);
   	//glPixelStorei(GL_UNPACK_ALIGNMENT,1);

      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                   texture.width.get(), texture.height.get(),
                   0, GL_BGRA, GL_UNSIGNED_BYTE, texture.pixelMap.get());


      //These need to be parameters in texture
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

      glGenerateMipmap(GL_TEXTURE_2D);

      texture.textureID.set(textureID);

   }
}
