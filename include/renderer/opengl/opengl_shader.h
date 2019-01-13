/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_OPENGL_SHADER_H
#define QGE_OPENGL_SHADER_H

#include "renderer/shader.h"

#include <GL/glew.h>

#include <map>
#include <string>
#include <vector>

namespace quasi_game_engine {

struct BufferIds {
  BufferIds(int vao_in, int attrib_in, int index_in)
      : vao(vao_in), attribute_id(attrib_in), index_id(index_in) {}
  int vao;
  int attribute_id;
  int index_id;
};

struct Uniform {
  Uniform(int id_in, GLenum type_in) : id(id_in), type(type_in){};
  Uniform() : id(-1), type(0){};
  int id;
  GLenum type;
};

class OpenGLShader {
 public:
  explicit OpenGLShader(const std::vector<std::string> file_paths);
  ~OpenGLShader() = default;

  // Getting rid of copy/move constructors/assignment operators (may need later)
  OpenGLShader(const OpenGLShader&) = default;
  OpenGLShader& operator=(const OpenGLShader&) = default;
  OpenGLShader(OpenGLShader&&) = default;
  OpenGLShader& operator=(OpenGLShader&&) = default;

  void SetActive() { glUseProgram((GLuint)program_id); };

  void SetUniformInteger(const std::string name, const int data);
  void SetUniformMatrix4(const std::string name, const std::vector<float> data);

 private:
  int program_id;
  std::map<std::string, Uniform> uniforms;

  void ParseShaderInputs();
};

}  // namespace quasi_game_engine

#endif  // QGE_OPENGL_SHADER_H
