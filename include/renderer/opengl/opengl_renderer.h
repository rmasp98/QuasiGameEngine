/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_OPENGL_RENDERER_H
#define QGE_OPENGL_RENDERER_H

#include "renderer/opengl/opengl_buffer_manager.h"
#include "renderer/opengl/opengl_shader.h"
#include "renderer/renderer.h"
#include "utils/data_types/qge_array.h"
#include "utils/data_types/qge_queue.h"

// Graphics libraries
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <functional>

namespace quasi_game_engine {

class OpenGLRenderer : public Renderer {
  /*------------------------------------------------------------------------------
    Description
  Notes
  - The should be a parent class with child classes for opengl and directx
  ------------------------------------------------------------------------------*/
 public:
  OpenGLRenderer();
  ~OpenGLRenderer() = default;

  // Getting rid of copy/move constructors/assignment operators (may need later)
  OpenGLRenderer(const OpenGLRenderer&) = delete;
  OpenGLRenderer& operator=(const OpenGLRenderer&) = delete;
  OpenGLRenderer(OpenGLRenderer&&) = delete;
  OpenGLRenderer& operator=(OpenGLRenderer&&) = delete;

  void LoadImage(const unsigned char* pixel_map, const int width,
                 const int height, const bool is_mipmap, int* texture_id) final;
  void LoadVertexAttribute(const QgeArray<float> attribute_data) final;
  void SetAttributePointer(const AttributeMetadata metadata,
                           const int stride) final;
  void LoadVertexIndices(const QgeArray<int> indices) final;

  void LoadImGui(const ImDrawList* draw_list,
                 const std::vector<AttributeMetadata> metadata,
                 const BufferIds* buffer_ids) final;

  void FillVao(int* vao, const std::function<void()>& func);

  void PushToRenderQueue(const DrawPipeline pipeline, const DrawConfig config);
  void Draw();

 private:
  std::vector<OpenGLShader> shader_list_;
  BufferManager buffer_manager_;

  QgeQueue<DrawConfig> draw_queue_[DP_SIZE];

  void SetRenderConfig(const DrawPipeline pipeline);
  void StandardDraw(DrawPipeline pipeline, OpenGLShader shader);
  void DrawLoop();
};

}  // namespace quasi_game_engine

#endif  // QGE_OPENGL_RENDERER_H
