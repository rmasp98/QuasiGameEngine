#include "renderer/renderManager.hpp"
#include "resource/texture.hpp"
#include "resource/resource.hpp"
#include "utils/logging/logger.hpp"



namespace xxx {

   RenderManager::RenderManager(LogWorker* logWorkerIn) {
      logger = new Logger("[ Render Manager ]", "logs/RenderManager.log", logWorkerIn);
   }


   RenderManager::~RenderManager() {
      delete logger;
   }


   void RenderManager::draw() {

   }



   bool RenderManager::initGraphics() {

      glewExperimental = true; //Find out what this means
      const GLenum glewErr = glewInit();
      if (glewErr != GLEW_OK) {
         LOG(LOG_FATAL, logger) << "Failed to initialise GLEW. "
                                << glewGetErrorString(glewErr);
                                
         return false;
      }

      glClearColor(0.0, 0.0, 0.4, 0.0);   //Set default backgrond to dark blue
      glEnable(GL_DEPTH_TEST);            //Enable depth testing of objects
      glDepthFunc(GL_LESS);               //Accept if fragment is closer
      glEnable(GL_CULL_FACE);             //Don't draw backfacing triangles#

      glEnable(GL_BLEND);
   	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      return true;
   }





   void RenderManager::loadTexture(Texture& texture) {

      LOG(LOG_TRACE, logger) << "Loading image to graphics";

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



   void RenderManager::loadObject(Mesh& mesh) {

      float *verts = NULL, *norms = NULL, *uvs = NULL;
      uint *faces = NULL, nFaces, nVerts, primitiveType=3, uvType=2;

      mesh.getMesh(&verts, &norms, &uvs, &faces, nVerts, nFaces);

      if ((verts != NULL) | (norms != NULL) | (uvs != NULL) | (faces != NULL)) {
         LOG(LOG_TRACE, logger) << "Loading object to graphics";

         uint vao = mesh.getVAO();
         glGenVertexArrays(1, &vao);
         glBindVertexArray(vao);

         // If the mesh has been vbo indexed already
         if (faces == NULL) {
            LOG(LOG_TRACE, logger) << "Creating a VBO from input verticies";
            // run vbo index method and reassign any valid mesh data
            // including numFaces and numVerts
         }

         //If verticies exist, bind them to graphics (potentially always should exist?)
         if (verts != NULL) {
            LOG(LOG_TRACE, logger) << "Binding vertex positions to graphics";

            GLuint vertID;
            glGenBuffers(1, &vertID);
            glBindBuffer(GL_ARRAY_BUFFER, vertID);
            glBufferData(GL_ARRAY_BUFFER,
                         nVerts * sizeof(float) * 3,
                         &verts[0],
                         GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

         }

         // If normal exist, bind them to graphics
         if (norms != NULL) {
            LOG(LOG_TRACE, logger) << "Binding vertex normals to graphics";

            GLuint normID;
            glGenBuffers(1, &normID);
            glBindBuffer(GL_ARRAY_BUFFER, normID);
            glBufferData(GL_ARRAY_BUFFER,
                         nVerts * sizeof(float) * 3,
                         &norms[0],
                         GL_STATIC_DRAW);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

         }

         //If texture coordinates exist, bind them to graphics
         if (uvs != NULL) {
            LOG(LOG_TRACE, logger) << "Binding texture coordiantes to graphics";

            GLuint uvID;
            glGenBuffers(1, &uvID);
            glBindBuffer(GL_ARRAY_BUFFER, uvID);
            glBufferData(GL_ARRAY_BUFFER,
                         nVerts * sizeof(float) * uvType,
                         &uvs[0],
                         GL_STATIC_DRAW);
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, uvType, GL_FLOAT, GL_FALSE, 0, (void*)0);

         }

         //Bind the VBO indexing to graphics
         LOG(LOG_TRACE, logger) << "Binding VBO inicies to graphics";

         GLuint faceID;
         glGenBuffers(1, &faceID);
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceID);
         glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                      nFaces * sizeof(uint) * primitiveType,
                      &faces[0] ,
                      GL_STATIC_DRAW);


         //Unbind VAO
         glBindVertexArray(0);

         mesh.setVAO(vao);

      } else {
         LOG(LOG_TRACE, logger) << "There is no data in this mesh to send to the graphics";
      }
   }

} // namespace xxx
