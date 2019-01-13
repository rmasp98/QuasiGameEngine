/*-----------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
-----------------------------------------------------------------------------*/

#include "renderer/opengl/opengl_renderer.h"

#include "utils/logging/log_capture.h"

namespace quasi_game_engine {

OpenGLRenderer::OpenGLRenderer() {
  glewExperimental = (GLboolean) true;  // Find out what this means
  const GLenum glew_error = glewInit();
  if (glew_error != GLEW_OK) {
    LOG(FATAL, RENDERER) << "Failed to initialise GLEW. "
                         << glewGetErrorString(glew_error);

    throw;
  }

  glClearColor(0.0, 0.0, 0.4, 0.0);  // Set default background to dark blue

  // These should be loaded from a config file (ORDER MATTERS)
  std::vector<std::string> shaders(2, "");
  shaders[0] = "/home/rmaspero/graphics/quasiGameEngine/shaders/main.vs";
  shaders[1] = "/home/rmaspero/graphics/quasiGameEngine/shaders/main.fs";
  shader_list_.push_back(OpenGLShader(shaders));

  shaders[0] = "/home/rmaspero/graphics/quasiGameEngine/shaders/ui.vs";
  shaders[1] = "/home/rmaspero/graphics/quasiGameEngine/shaders/ui.fs";
  shader_list_.push_back(OpenGLShader(shaders));
}

// TODO: Will need to implement different texparameters
void OpenGLRenderer::LoadImage(const unsigned char* pixel_map, const int width,
                               const int height, const bool is_mipmap,
                               int* texture_id) {
  LOG(INFO, RENDERER) << "Loading image to graphics";

  glGenTextures(1, (GLuint*)texture_id);
  glBindTexture(GL_TEXTURE_2D, (GLuint)*texture_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA,
               GL_UNSIGNED_BYTE, pixel_map);

  // These need to be parameters in texture
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  if (is_mipmap) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  }
}

void OpenGLRenderer::FillVao(int* vao, const std::function<void()>& func) {
  if (*vao == 0) glGenVertexArrays(1, (GLuint*)vao);

  buffer_manager_.BindVao(
      *vao);  // This will allow other functions to see that the vao is bound
  func();
  buffer_manager_.UnbindVao();
};

// Need to figure out how to template this
void OpenGLRenderer::LoadVertexAttribute(const QgeArray<float> attribute_data) {
  if (buffer_manager_.IsVaoBound()) {  // Ensure it is in a BindVao wrapper
    GLuint attribute_id;
    glGenBuffers(1, &attribute_id);
    buffer_manager_.AddBuffer(
        attribute_id);  // This ensures that we track and delete these buffers

    glBindBuffer(GL_ARRAY_BUFFER, attribute_id);
    glBufferData(GL_ARRAY_BUFFER, attribute_data.Size() * sizeof(float),
                 &attribute_data[0], GL_STATIC_DRAW);
  } else {
    LOG(ERROR, RENDERER) << "There is no VAO bound. Please use BindVao "
                         << "wrapper to ensure data is bound to a VAO";
    throw;
  }
}

void OpenGLRenderer::SetAttributePointer(const AttributeMetadata metadata,
                                         const int stride) {
  if (buffer_manager_.IsVaoBound()) {  // Ensure it is in a FillVao wrapper
    glEnableVertexAttribArray(metadata.name);
    glVertexAttribPointer(metadata.name, metadata.size, metadata.type,
                          metadata.normalised, stride,
                          (GLvoid*)metadata.offset);
  } else {
    LOG(ERROR, RENDERER) << "There is no VAO bound. Please use FillVao "
                         << "wrapper to ensure pointer is bound to a VAO";
    throw;
  }
}

void OpenGLRenderer::LoadVertexIndices(const QgeArray<int> indices) {
  // Need to check max number of index and also write a check for numVets
  if (buffer_manager_.IsVaoBound()) {
    GLuint index_id;
    glGenBuffers(1, &index_id);
    buffer_manager_.AddBuffer(
        index_id);  // This ensures that we track and delete these buffers

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.Size() * sizeof(int),
                 &indices[0], GL_STATIC_DRAW);

  } else {
    LOG(ERROR, RENDERER) << "There is no VAO bound. Please use FillVao "
                         << "wrapper to ensure pointer is bound to a VAO";
    throw;
  }
}

// Do not use this as an example of how to do things as it has been bodged to
// work with Imgui
void OpenGLRenderer::LoadImGui(const ImDrawList* draw_list,
                               const std::vector<AttributeMetadata> metadata,
                               const BufferIds* buffer_ids) {
  if (buffer_ids->vao == 0) {
    glGenVertexArrays(1, (GLuint*)&(buffer_ids->vao));
    LOG(INFO, RENDERER) << "Generating vertex array object";
  }
  buffer_manager_.BindVao(buffer_ids->vao);

  if (buffer_ids->attribute_id == 0 || buffer_ids->index_id == 0) {
    glGenBuffers(1, (GLuint*)&buffer_ids->attribute_id);
    glGenBuffers(1, (GLuint*)&buffer_ids->index_id);
  }
  // This ensures that we track and delete these buffers
  buffer_manager_.AddBuffer(buffer_ids->attribute_id);
  buffer_manager_.AddBuffer(buffer_ids->index_id);

  glBindBuffer(GL_ARRAY_BUFFER, buffer_ids->attribute_id);
  glBufferData(GL_ARRAY_BUFFER,
               (GLsizeiptr)draw_list->VtxBuffer.Size * sizeof(ImDrawVert),
               (const GLvoid*)draw_list->VtxBuffer.Data, GL_STREAM_DRAW);

  for (size_t i_meta = 0; i_meta < metadata.size(); ++i_meta) {
    SetAttributePointer(metadata[i_meta], sizeof(ImDrawVert));
  }

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_ids->index_id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               (GLsizeiptr)draw_list->IdxBuffer.Size * sizeof(ImDrawIdx),
               (const GLvoid*)draw_list->IdxBuffer.Data, GL_STREAM_DRAW);

  buffer_manager_.UnbindVao();
}

void OpenGLRenderer::PushToRenderQueue(const DrawPipeline pipeline,
                                       const DrawConfig config) {
  draw_queue_[pipeline].Push(config);
}

void OpenGLRenderer::Draw() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear screen
  for (int i = 0; i < DP_SIZE; ++i) {
    if (i == DP_DEFAULT || i == DP_IMGUI) {
      StandardDraw((DrawPipeline)i, shader_list_[i]);
    }
  }

  glDisable(
      GL_SCISSOR_TEST);  // Stops flickering if scissor is enabled from GUI
}

void OpenGLRenderer::StandardDraw(DrawPipeline pipeline, OpenGLShader shader) {
  shader.SetActive();

  SetRenderConfig(pipeline);

  int queue_size = draw_queue_[pipeline].Size();
  for (int j = 0; j < queue_size; ++j) {
    DrawConfig config = draw_queue_[pipeline].Pop();

    shader.SetUniformMatrix4("MVP", config.mvp);

    if (config.texture_id != 0) {
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, (GLuint)config.texture_id);
    }

    if (config.scissor_box.size() == 4) {
      glScissor(config.scissor_box[0], config.scissor_box[1],
                config.scissor_box[2], config.scissor_box[3]);
    }

    glBindVertexArray((GLuint)config.vao);
    glDrawElements(GL_TRIANGLES, (GLsizei)config.num_elements,
                   pipeline == DP_IMGUI ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT,
                   (GLvoid*)config.offset);
  }
}

// This probably won't work because a pipeline might need multiple
// configurations
void OpenGLRenderer::SetRenderConfig(const DrawPipeline pipeline) {
  if (pipeline == DP_DEFAULT) {
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_BLEND);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  } else if (pipeline == DP_IMGUI) {
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
}

}  // namespace quasi_game_engine
