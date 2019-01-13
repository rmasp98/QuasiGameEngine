/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_RENDERER_H
#define QGE_RENDERER_H

#include "renderer/opengl/opengl_shader.h"
#include "utils/data_types/qge_array.h"

#include <functional>
#include <string>
#include <vector>

namespace quasi_game_engine {

enum DrawPipeline { DP_DEFAULT = 0, DP_IMGUI = 1, DP_SIZE = 2 };

struct DrawConfig {
  int vao;
  int num_elements;
  unsigned short* offset;
  int texture_id;
  std::vector<int> scissor_box;
  std::vector<float> mvp;
};

class Renderer {
  /*------------------------------------------------------------------------------
    Interface for graphics API wrappers
  Notes
  - The should be a parent class with child classes for opengl and directx
  ------------------------------------------------------------------------------*/
 public:
  virtual ~Renderer() = default;

  // Getting rid of copy/move constructors/assignment operators (may need later)
  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;
  Renderer(Renderer&&) = delete;
  Renderer& operator=(Renderer&&) = delete;

  virtual void LoadImage(const unsigned char* pixel_map, const int width,
                         const int height, const bool is_mipmap,
                         int* texture_id) = 0;

  virtual void LoadVertexAttribute(const QgeArray<float> attribute_data) = 0;

  virtual void SetAttributePointer(const AttributeMetadata metadata,
                                   const int stride) = 0;

  virtual void LoadVertexIndices(const QgeArray<int> indices) = 0;

  virtual void LoadImGui(const ImDrawList* draw_list,
                         const std::vector<AttributeMetadata> metadata,
                         const BufferIds* buffer_ids) = 0;

  virtual void FillVao(int* vao, const std::function<void()>& func) = 0;

  virtual void PushToRenderQueue(const DrawPipeline pipeline,
                                 const DrawConfig config) = 0;

  virtual void Draw() = 0;

 protected:
  Renderer() = default;
};

}  // namespace quasi_game_engine

#endif  // QGE_RENDERER_H
