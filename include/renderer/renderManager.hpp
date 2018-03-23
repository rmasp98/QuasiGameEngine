#ifndef XXX_RENDER_MANAGER_HPP
#define XXX_RENDER_MANAGER_HPP


//Graphics libraries
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include <GL/gl.h>




namespace xxx {
   class Texture;
   class Mesh;
   class Logger;
   class LogWorker;

   /*Notes----------------------------------------------------------------------
   - The should be a parent class with child classes for opengl and directx
   ---------------------------------------------------------------------------*/
   class RenderManager {
   public:
      RenderManager(LogWorker* logWorkerIn);
      ~RenderManager();

      bool initGraphics();
      void draw();
      void loadTexture(Texture& texture);
      void loadObject(Mesh& mesh);

   private:
      Logger* logger;

   };


   class OpenGLManager : public RenderManager {

   };



}

#endif // XXX_RENDER_MANAGER_HPP
