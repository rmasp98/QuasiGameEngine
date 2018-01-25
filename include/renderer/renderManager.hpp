#ifndef XXX_RENDER_MANAGER_HPP
#define XXX_RENDER_MANAGER_HPP


//Graphics libraries
#include <GL/glew.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

//This may need to be better organised
//#include "resource/texture.hpp"


namespace xxx {
   class Texture;

   /*Notes----------------------------------------------------------------------
   - The should be a parent class with child classes for opengl and directx
   ---------------------------------------------------------------------------*/
   class RenderManager {
   public:
      RenderManager();
      ~RenderManager();

      int initGraphics();
      void draw();
      GLFWwindow* getWindow() { return window; };
      void loadTexture(Texture& texture);

   private:
      GLFWwindow* window;

   };


   class OpenGLManager : public RenderManager {

   };



}

#endif // XXX_RENDER_MANAGER_HPP
