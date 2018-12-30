/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_SHADER_H
#define QGE_SHADER_H

#include <GL/glew.h>
#include <string>
#include <vector>
#include <map>

namespace quasi_game_engine {

struct Uniform {
  Uniform(int id_in, GLenum type_in) : id(id_in), type(type_in) {};
  Uniform() : id(-1), type(0) {};
  int id;
  GLenum type;
};

//Need a vector of struct containing attrib index, size and type for each element
// struct AttributeMetadata {
//   AttributeMetadata(int attrib_index, int attrib_size, GLenum attrib_type, size_t attrib_offset, bool something) 
//       : index(attrib_index), type(attrib_type), size(attrib_size), offset(attrib_offset), somethidgnlb(something) {}
//   int index;
//   GLenum type;
//   int size;
//   size_t offset;
//   bool somethidgnlb;
// };

struct AttributeMetadata {
  AttributeMetadata(std::string attrib_name, int attrib_size, GLenum attrib_type, size_t attrib_offset, bool something) 
      : name(attrib_name), type(attrib_type), size(attrib_size), offset(attrib_offset), somethidgnlb(something) {}
  std::string name;
  GLenum type;
  int size;
  size_t offset;
  bool somethidgnlb;
};

class Shader {
 public:
  Shader() = default;
  virtual ~Shader() = default;
  virtual void SetActive() = 0;
  virtual uint GetProgram() = 0;

  virtual void SetUniformInteger(std::string name, int data) = 0;
  virtual void SetUniformMatrix4(std::string name, std::vector<float> data) = 0;

  virtual bool SetAttributePointer(std::vector<AttributeMetadata> metadata,
      int stride, int* vao) = 0;

};


class OpenGLShader : public Shader {
 public:
  OpenGLShader(std::vector<std::string> file_paths);
  ~OpenGLShader() = default;

  //Getting rid of copy/move constructors/assignment operators (may need later)
  OpenGLShader(const OpenGLShader&) = delete;
  OpenGLShader& operator=(const OpenGLShader&) = delete;
  OpenGLShader(OpenGLShader&&) = delete;
  OpenGLShader& operator=(OpenGLShader&&) = delete;

  void SetActive() { glUseProgram(program_id); };

  //Remove this when everything in working
  uint GetProgram() { return program_id; };
  
  void SetUniformInteger(std::string name, int data);
  void SetUniformMatrix4(std::string name, std::vector<float> data);

  bool SetAttributePointer(std::vector<AttributeMetadata> metadata,
    int stride, int* vao) final;

 private:
  uint program_id; // See if we can get away with changing to int
  std::map<std::string, Uniform> uniforms;
  std::map<std::string, Uniform> attributes;

  void ParseShaderInputs();
};


}  // namespace quasi_game_engine

#endif  // QGE_SHADER_H
