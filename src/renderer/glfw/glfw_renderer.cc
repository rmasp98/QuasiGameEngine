#include "renderer/glfw/glfw_renderer.h"
//#include "resource/texture.h"
//#include "resource/resource.h"

namespace quasi_game_engine {

GlfwRenderer::GlfwRenderer(LogWorker* logWorkerIn)
    : logger_("[     Renderer     ]", "logs/Renderer.log", logWorkerIn) {}


void GlfwRenderer::Draw() {

}


bool GlfwRenderer::InitGraphics() {
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



bool GlfwRenderer::LoadImage(const unsigned char* pixel_map, int width,
                             int height, bool is_mipmap, uint* texture_id) {
  LOG(LOG_INFO, &logger_) << "Loading image to graphics";

  glGenTextures(1, texture_id);
  glBindTexture(GL_TEXTURE_2D, *texture_id);
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

  return true;
}




// Need to check the performance of this as binding VAO multiple times per mesh
bool GlfwRenderer::LoadVertexAttribute(const float* attribute_data,
                                       int num_verts,
                                       Attribute attribute,
                                       uint* vao) {
  // Need to check max number of index and also write a check for numVets
  if ((attribute_data != nullptr) && (attribute.num_components < 5)
      && (attribute.attribute_index < 10)) {
    if (*vao == 0) {
      glGenVertexArrays(1, vao);
      LOG(LOG_INFO, &logger_) << "Generating vertex array object";
    }

    glBindVertexArray(*vao);
    GLuint attribute_id;
    glGenBuffers(1, &attribute_id);
    glBindBuffer(GL_ARRAY_BUFFER, attribute_id);
    glBufferData(GL_ARRAY_BUFFER,
                 num_verts * sizeof(float) * attribute.num_components,
                 &attribute_data[0],
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray((GLuint)attribute.attribute_index);
    glVertexAttribPointer((GLuint)attribute.attribute_index,
                          attribute.num_components,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          nullptr);

    glBindVertexArray(0);

    return true;
  } else {
    LOG(LOG_ERROR, &logger_) << "Cannot load Vertex attribute to graphics "
                             << "Vertex attribute is corrupt.";
    return false;
  }

}


bool GlfwRenderer::LoadVertexIndices(const int *indices,
                                     int num_indices,
                                     int num_components,
                                     uint* vao) {
  // Need to check max number of index and also write a check for numVets
  if ((indices != nullptr) && (num_components < 5)) {
    if (*vao == 0) {
      glGenVertexArrays(1, vao);
      LOG(LOG_INFO, &logger_) << "Generating vertex array object";
    }

    glBindVertexArray(*vao);

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

} // namespace quasi_game_engine
