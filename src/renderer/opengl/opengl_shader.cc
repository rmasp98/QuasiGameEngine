/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#include "renderer/opengl/opengl_shader.h"

#include "utils/logging/log_capture.h"

#include <fstream>
#include <iostream>
#include <algorithm>

namespace quasi_game_engine {

OpenGLShader::OpenGLShader(std::vector<std::string> file_paths) {
  GLint result = GL_FALSE;
  GLint info_log_length;
  std::vector<GLuint> ids;
  program_id = glCreateProgram();

  GLuint vert_index = 0, geometry_index = 0, fragment_index = 0;

  // Loop over each shader
  for (GLuint i_shader = 0; i_shader < file_paths.size(); i_shader++) {
    // Determine which tyoe of shader
    GLuint shader_id = 0;
    if ((file_paths[i_shader].compare(file_paths[i_shader].size() - 3, 3,
                                      ".vs") == 0) &&
        (!vert_index++))
      shader_id = glCreateShader(GL_VERTEX_SHADER);
    else if ((file_paths[i_shader].compare(file_paths[i_shader].size() - 3, 3,
                                           ".gs") == 0) &&
             (!geometry_index++))
      shader_id = glCreateShader(GL_GEOMETRY_SHADER);
    else if ((file_paths[i_shader].compare(file_paths[i_shader].size() - 3, 3,
                                           ".fs") == 0) &&
             (!fragment_index++))
      shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    else {
      LogCapture(ERROR, RENDERER) << "Shaders do not have the extension: " <<
          ".vs, .gs, or .fs; or two of the same shader is loaded";
      throw;
    }

    // Read code from shader
    std::string shader_code;
    std::ifstream shader_stream(file_paths[i_shader].c_str(), std::ios::in);
    if (shader_stream.is_open()) {
      std::string line = "";
      while (getline(shader_stream, line)) shader_code += "\n" + line;

      shader_stream.close();
    } else {
      LogCapture(ERROR, RENDERER) << "Impossible to open " << file_paths[i_shader]
          << ". Are you in the right directory? Don't forget to read the FAQ!";
      throw;
    }

    // Compile shader
    LogCapture(INFO, RENDERER) << "Compiling shader: " << file_paths[i_shader];
    char const* source_pointer = shader_code.c_str();
    glShaderSource(shader_id, 1, &source_pointer, NULL);
    glCompileShader(shader_id);

    // Check shader
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length > 0) {
      GLchar* shader_error_message = new GLchar[info_log_length + 1];
      glGetShaderInfoLog(shader_id, info_log_length, NULL,
                         shader_error_message);
      LogCapture(ERROR, RENDERER) << shader_error_message;
      delete[] shader_error_message;
    }

    // Attach shader and add to array from later deletion
    glAttachShader(program_id, shader_id);
    ids.push_back(shader_id);
  }

  if (vert_index && fragment_index) {
    // Link the program
    LogCapture(INFO, RENDERER) << "Linking program";
    glLinkProgram(program_id);

    // Error check the linking
    glGetProgramiv(program_id, GL_LINK_STATUS, &result);
    glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length > 0) {
      GLchar* program_error_message = new GLchar[info_log_length + 1];
      glGetProgramInfoLog(program_id, info_log_length, NULL,
                          program_error_message);
      LogCapture(ERROR, RENDERER) << program_error_message;
    }

    // Delete the shaders
    for (GLuint i_shader = 0; i_shader < ids.size(); i_shader++)
      glDeleteShader(ids[i_shader]);

    ParseShaderInputs();
  } else {
    LogCapture(ERROR, RENDERER) << "You have not loaded at least a vertex "
        << "and fragment shader";
    throw;
  }
}


void OpenGLShader::ParseShaderInputs() {
  int count;
  std::string name_str;
  GLint size; GLenum type; GLsizei length; 

  glGetProgramiv(program_id, GL_ACTIVE_UNIFORMS, &count);
  for (int i_uniform=0; i_uniform < count; ++i_uniform) {
    char name[16];
    glGetActiveUniform(program_id, (GLuint)i_uniform, 16, &length, 
        &size, &type, name);

    GLuint id = glGetUniformLocation(program_id, name);
    uniforms.insert(std::pair<std::string, Uniform>(name, Uniform(id, type)));
  }

  glGetProgramiv(program_id, GL_ACTIVE_ATTRIBUTES, &count);
  for (int i_uniform=0; i_uniform < count; ++i_uniform) {
    char name[16];
    glGetActiveAttrib(program_id, (GLuint)i_uniform, 16, &length,
        &size, &type, name);
    
    GLuint id = glGetAttribLocation(program_id, name);
    attributes.insert(std::pair<std::string, Uniform>(name, Uniform(id, type)));
  }
}


void OpenGLShader::SetUniformInteger(std::string name, int data) {
  if (uniforms.find(name) != uniforms.end() && uniforms[name].type == GL_INT) {
    glUniform1i(uniforms[name].id, data);
  }
}


void OpenGLShader::SetUniformMatrix4(std::string name, std::vector<float> data) {
  if (uniforms.find(name) != uniforms.end() && 
      uniforms[name].type == GL_FLOAT_MAT4 && data.size() == 16) {
    glUniformMatrix4fv(uniforms[name].id, 1, GL_FALSE, &data[0]);
  }
}


bool OpenGLShader::SetAttributePointer(
    std::vector<AttributeMetadata> metadata, int stride, int* vao) {

  //OpenGL only accepts GLuint for indices but we work in integers
  GLuint tempVao = *vao;

  // Need to check max number of index and also write a check for numVets
  if (tempVao == 0) {
    glGenVertexArrays(1, &tempVao);
    LOG(INFO, RENDERER) 
        << "Generating vertex array object";

    *vao = (int)tempVao;
  }

  glBindVertexArray(tempVao); //Bind VAO

  for (size_t i_meta=0; i_meta < metadata.size(); ++i_meta) {
    GLuint index = attributes[metadata[i_meta].name].id;
    if (index >= 0 && index < 10) {
      glEnableVertexAttribArray(index);
      glVertexAttribPointer(index,
                            metadata[i_meta].size,
                            metadata[i_meta].type,
                            metadata[i_meta].somethidgnlb,
                            20,//total_size,
                            (GLvoid*)metadata[i_meta].offset);
    }
  }

  return true;

}


}  // namespace quasi_game_engine
