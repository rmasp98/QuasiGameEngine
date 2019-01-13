/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_SHADER_H
#define QGE_SHADER_H

#include "imgui/imgui.h"

#include <GL/glew.h>  //TODO: This should be removed when I have sorted the structs out

#include <map>
#include <string>
#include <vector>

namespace quasi_game_engine {

// This represents what each attribute should be in the shader
enum AttributeName {
  ATTR_POSITION = 0,
  ATTR_UV = 1,
  ATTR_NORMAL = 2,
  ATTR_COLOUR = 3
};

struct AttributeMetadata {
  AttributeMetadata(AttributeName attrib_name, int attrib_size,
                    GLenum attrib_type, size_t attrib_offset,
                    bool attrib_normalised)
      : name(attrib_name),
        type(attrib_type),
        size(attrib_size),
        offset(attrib_offset),
        normalised(attrib_normalised) {}
  AttributeName name;
  GLenum type;
  int size;
  size_t offset;
  bool normalised;
};

class Shader {
 public:
  virtual ~Shader() = default;
  virtual void SetActive() = 0;

  virtual void SetUniformInteger(const std::string name, const int data) = 0;
  virtual void SetUniformMatrix4(const std::string name,
                                 const std::vector<float> data) = 0;

  virtual void SetAttributePointer(const AttributeMetadata metadata,
                                   const int stride) = 0;

 protected:
  Shader() = default;
};

}  // namespace quasi_game_engine

#endif  // QGE_SHADER_H
