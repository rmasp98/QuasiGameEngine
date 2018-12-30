/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#include "renderer/opengl/opengl_renderer.h"

#include "utils/logging/log_capture.h"

namespace quasi_game_engine {

//TODO: Need to implement
void OpenGLRenderer::Draw() {

}


bool OpenGLRenderer::InitGraphics() {
  glewExperimental = (GLboolean)true; //Find out what this means
  const GLenum glew_error = glewInit();
  if (glew_error != GLEW_OK) {
    LOG(FATAL, RENDERER) 
        << "Failed to initialise GLEW. "
        << glewGetErrorString(glew_error);

    return false;
  }

  glClearColor(0.0, 0.0, 0.4, 0.0);   //Set default background to dark blue
  return true;
}

Shader* OpenGLRenderer::LoadShaders(std::vector<std::string> file_paths) {
  shader_list.push_back(new OpenGLShader(file_paths));
  //TODO: When I have fixed everything else, I need to output my own id as below
  //return shader_list.size() - 1;
  return shader_list.back();
}


//TODO: Will need to implement different texparameters
bool OpenGLRenderer::LoadImage(const unsigned char* pixel_map, int width,
                             int height, bool is_mipmap, int* texture_id) {
  LOG(INFO, RENDERER) << "Loading image to graphics";

  GLuint temp_id;
  glGenTextures(1, &temp_id);
  glBindTexture(GL_TEXTURE_2D, temp_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
               0, GL_BGRA, GL_UNSIGNED_BYTE, pixel_map);

  // These need to be parameters in texture
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  if (is_mipmap) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  }

  *texture_id = (int)temp_id;
  return true;
}





//TODO: Need to check the performance of this as binding VAO multiple times per mesh
//May be worth making function that binds VAO
bool OpenGLRenderer::LoadVertexAttribute(const QgeArray<float> attribute_data,
                                         int attribute_index, int* vao) {
  //OpenGL only accepts GLuint for indices but we work in integers
  GLuint tempVao = (GLuint)*vao;

  // Need to check max number of index and also write a check for numVerts
  if (attribute_index >= 0 && attribute_index < 10) {
    if (tempVao == 0) {
      glGenVertexArrays(1, &tempVao);
      LOG(INFO, RENDERER) 
          << "Generating vertex array object";

      *vao = (int)tempVao;
    }

    glBindVertexArray(tempVao); //Bind VAO

    GLuint attribute_id;
    glGenBuffers(1, &attribute_id);  //This needs to be stored and deleted when no longer needed
    glBindBuffer(GL_ARRAY_BUFFER, attribute_id);
    //Transfer data to GPU
    glBufferData(GL_ARRAY_BUFFER,
                 attribute_data.Size() * sizeof(float),
                 &attribute_data[0],
                 GL_STATIC_DRAW);
    //What does this do?
    glEnableVertexAttribArray((GLuint)attribute_index);
    glVertexAttribPointer((GLuint)attribute_index,
                          attribute_data.GetNumComponents(),
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          nullptr);

    glBindVertexArray(0); //Unbind VAO

    return true;
  } else {
    LOG(ERROR, RENDERER) 
        << "Cannot load Vertex attribute to graphics. "
        << "Vertex attribute is corrupt.";
    return false;
  }

}


bool OpenGLRenderer::LoadVertexIndices(const QgeArray<int> indices, int* vao) {
  //OpenGL only accepts GLuint for indices but we work in integers
  GLuint tempVao = *vao;

  // Need to check max number of index and also write a check for numVets
  if (indices.GetNumComponents() < 5) {
    if (tempVao == 0) {
      glGenVertexArrays(1, &tempVao);
      LOG(INFO, RENDERER) 
          << "Generating vertex array object";

      *vao = (int)tempVao;
    }

    glBindVertexArray(tempVao); //Bind VAO

    GLuint index_id;
    glGenBuffers(1, &index_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.Size() * sizeof(int),
                 &indices[0],
                 GL_STATIC_DRAW);

    glBindVertexArray(0); //Unbind VAO

    return true;
  } else {
    LOG(ERROR, RENDERER) 
        << "Cannot load Vertex attribute to graphics "
        << "Vertex attribute is corrupt.";
        
    return false;
  }
}


bool OpenGLRenderer::DrawImGui(const ImDrawData* draw_data, 
    std::vector<int> buffer_size, int* data_id, int* index_id) {
  
  if (*data_id == 0 || *index_id == 0) {
    glGenBuffers(1, (GLuint*)data_id);
    glGenBuffers(1, (GLuint*)index_id);
  }

  // Backup GL state
  GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
  
  // Setup viewport, orthographic projection matrix
  glViewport(0, 0, (GLsizei)buffer_size[0], (GLsizei)buffer_size[1]);
  glBindSampler(0, 0); // Rely on combined texture/sampler state.

  // Draw
  for (int n = 0; n < draw_data->CmdListsCount; ++n) {
    const ImDrawList* cmd_list = draw_data->CmdLists[n];
    const ImDrawIdx* idx_buffer_offset = 0;

    glBindBuffer(GL_ARRAY_BUFFER, *data_id);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), (const GLvoid*)cmd_list->VtxBuffer.Data, GL_STREAM_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *index_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), (const GLvoid*)cmd_list->IdxBuffer.Data, GL_STREAM_DRAW);

    for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; ++cmd_i) {
      const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
      if (pcmd->UserCallback) {
        pcmd->UserCallback(cmd_list, pcmd);
      } else {
        glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
        glScissor((int)pcmd->ClipRect.x, (int)(buffer_size[1] - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
        glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer_offset);
      }
      
      idx_buffer_offset += pcmd->ElemCount;
    }
  }

  glDisable(GL_SCISSOR_TEST);
  glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);

  return true;
}

} // namespace quasi_game_engine
