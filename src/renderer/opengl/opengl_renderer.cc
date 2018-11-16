/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#include "renderer/opengl/opengl_renderer.h"

namespace quasi_game_engine {

OpenGLRenderer::OpenGLRenderer(LogWorker* log_worker)
    : logger_("Renderer", "logs/Renderer.log", log_worker) {}


//TODO: Need to implement
void OpenGLRenderer::Draw() {

}


bool OpenGLRenderer::InitGraphics() {
  glewExperimental = (GLboolean)true; //Find out what this means
  const GLenum glew_error = glewInit();
  if (glew_error != GLEW_OK) {
    LOG(LOG_FATAL, &logger_) << "Failed to initialise GLEW. "
                             << glewGetErrorString(glew_error);
    return false;
  }

  glClearColor(0.0, 0.0, 0.4, 0.0);   //Set default background to dark blue
  return true;
}


//TODO: Will need to implement different texparameters
bool OpenGLRenderer::LoadImage(const unsigned char* pixel_map, int width,
                             int height, bool is_mipmap, int* texture_id) {
  LOG(LOG_INFO, &logger_) << "Loading image to graphics";

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

  // Need to check max number of index and also write a check for numVets
  if (attribute_index < 10) {
    if (tempVao == 0) {
      glGenVertexArrays(1, &tempVao);
      LOG(LOG_INFO, &logger_) << "Generating vertex array object";
      *vao = (int)tempVao;
    }

    glBindVertexArray(tempVao); //Bind VAO

    GLuint attribute_id;
    glGenBuffers(1, &attribute_id);
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
    LOG(LOG_ERROR, &logger_) << "Cannot load Vertex attribute to graphics "
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
      LOG(LOG_INFO, &logger_) << "Generating vertex array object";
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
    LOG(LOG_ERROR, &logger_) << "Cannot load Vertex attribute to graphics "
                             << "Vertex attribute is corrupt.";
    return false;
  }
}

} // namespace quasi_game_engine
