#ifndef QGE_RENDER_MANAGER_H
#define QGE_RENDER_MANAGER_H


#include "utils/logging/logger.h"

//Graphics libraries
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include <GL/gl.h>




namespace quasi_game_engine {
   class Texture;
   class Mesh;
   class Logger;
   class LogWorker;

   //need to redo this later to make it better
   struct RenderConfig {
      RenderConfig() {};
      RenderConfig(bool cull_in, bool depth_in, bool scissor_in, bool blend_in,
                   int src_func_in, int dest_func_in, int blend_equation_in,
                   int polygon_mode_in) : cull_face(cull_in), depth_test(depth_in),
                   scissor_test(scissor_in), blend(blend_in), blend_src_func(src_func_in),
                   blend_dest_func(dest_func_in), blend_equation(blend_equation_in),
                   polygon_mode(polygon_mode_in) {};

      bool cull_face;
      bool depth_test;
      bool scissor_test;
      bool blend;

      int blend_src_func;
      int blend_dest_func;
      int blend_equation;
      int polygon_mode;
   };

   //Need to update these types to make universal
   struct Attribute {
      int attribute_index;
      int num_components;
      //GL_TYPE dataType;
      //GL_BOOL normalised;

   };


   /*Notes----------------------------------------------------------------------
   - The should be a parent class with child classes for opengl and directx
   ---------------------------------------------------------------------------*/
   class RenderManager {
   public:
      RenderManager(LogWorker* log_worker);
      ~RenderManager();

      bool InitGraphics();
      void Draw();

      bool LoadImage(uint *texture_id, unsigned char *pixel_map,
                     int width, int height, bool is_mipmap);

      bool LoadVertexAttribute(uint &vao, const float *attribute, int num_verts,
                               int num_components, int attribute_index);

      bool LoadVertexIndices(uint &vao, const int *indices, int num_indices,
                             int num_components);

      void SetRenderConfig(RenderConfig render_config);


   private:
      Logger logger_;

   };


   class OpenGLManager : public RenderManager {

   };



} // namespace quasi_game_engine

#endif // QGE_RENDER_MANAGER_H
