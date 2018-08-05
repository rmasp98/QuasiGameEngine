#ifndef QGE_GLFW_RENDERER_H
#define QGE_GLFW_RENDERER_H

#include "renderer/renderer.h"
#include "utils/logging/logger.h"

//Graphics libraries
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace quasi_game_engine {
class LogWorker;

class GlfwRenderer : public Renderer {
/*Notes----------------------------------------------------------------------
- The should be a parent class with child classes for opengl and directx
---------------------------------------------------------------------------*/
 public:
  explicit GlfwRenderer(LogWorker *log_worker);
  ~GlfwRenderer() = default;

  //Getting rid of copy/move constructors/assignment operators (may need later)
  GlfwRenderer(const GlfwRenderer&) = delete;
  GlfwRenderer& operator=(const GlfwRenderer&) = delete;
  GlfwRenderer(GlfwRenderer&&) = delete;
  GlfwRenderer& operator=(GlfwRenderer&&) = delete;

  bool InitGraphics() final;
  void Draw() final;
  bool LoadImage(const unsigned char *pixel_map, int width, int height,
                 bool is_mipmap, uint* texture_id) final;
  bool LoadVertexAttribute(const float* attribute_data, int num_verts,
                           Attribute attribute, uint* vao) final;

  bool LoadVertexIndices(const int *indices, int num_indices,
                         int num_components, uint* vao) final;

 private:
  Logger logger_;
};

} // namespace quasi_game_engine

#endif // QGE_GLFW_RENDERER_H
