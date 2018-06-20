#include "renderer/render_manager.h"
#include "resource/texture.h"
#include "resource/resource.h"




namespace quasi_game_engine {

   RenderManager::RenderManager(LogWorker* logWorkerIn)
      : logger_("[  Render Manager  ]", "logs/RenderManager.log", logWorkerIn) {}


   RenderManager::~RenderManager() {}


   void RenderManager::Draw() {

   }



   bool RenderManager::InitGraphics() {

      glewExperimental = true; //Find out what this means
      const GLenum glew_error = glewInit();
      if (glew_error != GLEW_OK) {
         LOG(LOG_FATAL, &logger_) << "Failed to initialise GLEW. "
                                << glewGetErrorString(glew_error);

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



   bool RenderManager::LoadImage(uint *texture_id, unsigned char *pixel_map,
                                 int width, int height, bool is_mipmap)
   {

      LOG(LOG_INFO, &logger_) << "Loading image to graphics";

      glGenTextures(1, texture_id);
      glBindTexture(GL_TEXTURE_2D, *texture_id);

      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                   width, height,
                   0, GL_BGRA, GL_UNSIGNED_BYTE, pixel_map);

      // These need to be parameters in texture
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      if (is_mipmap) {
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
         glGenerateMipmap(GL_TEXTURE_2D);
      } else
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

      return true;

   }




   // Need to check the performance of this as binding VAO multiple times per mesh
   bool RenderManager::LoadVertexAttribute(uint &vao, const float *attribute_data,
                                           int num_verts, int num_components,
                                           int attribute_index) {

      // Need to check max number of index and also write a check for numVets
      if ((attribute_data != nullptr) && (num_components < 5) && (attribute_index < 10)) {
         if (vao == 0) {
            glGenVertexArrays(1, &vao);
            LOG(LOG_INFO, &logger_) << "Generating vertex array object";
         }

         glBindVertexArray(vao);

         GLuint attribute_id;
         glGenBuffers(1, &attribute_id);
         glBindBuffer(GL_ARRAY_BUFFER, attribute_id);
         glBufferData(GL_ARRAY_BUFFER,
                      num_verts * sizeof(float) * num_components,
                      &attribute_data[0],
                      GL_STATIC_DRAW);
         glEnableVertexAttribArray(attribute_index);
         glVertexAttribPointer(attribute_index, num_components, GL_FLOAT, GL_FALSE, 0, (void*)0);

         glBindVertexArray(0);

         return true;
      } else {
         LOG(LOG_ERROR, &logger_) << "Cannot load Vertex attribute to graphics "
                                 << "Vertex attribute is corrupt.";

         return false;
      }

   }


   bool RenderManager::LoadVertexIndices(uint &vao, const int *indices, int num_indices,
                                         int num_components) {

      // Need to check max number of index and also write a check for numVets
      if ((indices != nullptr) && (num_components < 5)) {
         if (vao == 0) {
            glGenVertexArrays(1, &vao);
            LOG(LOG_INFO, &logger_) << "Generating vertex array object";
         }

         glBindVertexArray(vao);

         GLuint index_id;
         glGenBuffers(1, &index_id);
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_id);
         glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                      num_indices * sizeof(uint) * num_components,
                      &indices[0],
                      GL_STATIC_DRAW);

         glBindVertexArray(0);

         return true;
      } else {
         LOG(LOG_ERROR, &logger_) << "Cannot load Vertex attribute to graphics "
                                 << "Vertex attribute is corrupt.";

         return false;
      }

   }



   void RenderManager::SetRenderConfig(RenderConfig render_config) {

      if (render_config.cull_face)
         glEnable(GL_CULL_FACE);
      else
         glDisable(GL_CULL_FACE);


      if (render_config.depth_test)
         glEnable(GL_DEPTH_TEST);
      else
         glDisable(GL_DEPTH_TEST);

      if (render_config.scissor_test)
         glEnable(GL_SCISSOR_TEST);
      else
         glDisable(GL_SCISSOR_TEST);

      if (render_config.blend) {
         glEnable(GL_BLEND);

         glBlendEquation(render_config.blend_equation);
         glBlendFunc(render_config.blend_src_func, render_config.blend_dest_func);
      } else
         glDisable(GL_BLEND);

      // This may need full functionality
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   }

} // namespace quasi_game_engine
