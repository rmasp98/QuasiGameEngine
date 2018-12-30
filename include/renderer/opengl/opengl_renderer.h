/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_OPENGL_RENDERER_H
#define QGE_OPENGL_RENDERER_H

#include "renderer/renderer.h"
#include "renderer/opengl/opengl_shader.h"
#include "utils/qge_array.h"


//Graphics libraries
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace quasi_game_engine {
class LogWorker;

class OpenGLRenderer : public Renderer {
/*------------------------------------------------------------------------------
  Description
Notes
- The should be a parent class with child classes for opengl and directx
------------------------------------------------------------------------------*/
 public:
  OpenGLRenderer() = default;
  ~OpenGLRenderer() = default;

  //Getting rid of copy/move constructors/assignment operators (may need later)
  OpenGLRenderer(const OpenGLRenderer&) = delete;
  OpenGLRenderer& operator=(const OpenGLRenderer&) = delete;
  OpenGLRenderer(OpenGLRenderer&&) = delete;
  OpenGLRenderer& operator=(OpenGLRenderer&&) = delete;

  bool InitGraphics() final;
  void Draw() final;
  Shader* LoadShaders(std::vector<std::string> file_paths);
  void ActivateShader(int program_id) { 
    if (program_id >= 0 && program_id < shader_list.size()) {
      shader_list[program_id]->SetActive();
    }
  };
  bool LoadImage(const unsigned char *pixel_map, int width, int height,
      bool is_mipmap, int* texture_id) final;
  bool LoadVertexAttribute(const QgeArray<float> attribute_data,
      int attribute_index, int* vao) final;
  bool LoadVertexIndices(const QgeArray<int> indices, int* vao) final;

  bool DrawImGui(const ImDrawData* draw_list, std::vector<int> buffer_size, 
      int* data_id, int* index_id) final;

 private:
  std::vector<Shader*> shader_list;
};

} // namespace quasi_game_engine

#endif // QGE_OPENGL_RENDERER_H
