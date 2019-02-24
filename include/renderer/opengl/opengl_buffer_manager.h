/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_OPENGL_BUFFER_MANAGER_H
#define QGE_OPENGL_BUFFER_MANAGER_H

// Graphics libraries
#include <GL/glew.h>

#include <map>
#include <vector>

namespace quasi_game_engine {

class BufferManager {
  /*-----------------------------------------------------------------------------
    This is a class to make sure we track and delete all OpenGL buffers
  Notes
  -
  -----------------------------------------------------------------------------*/
 public:
  BufferManager() = default;
  ~BufferManager() {
    for (auto& elem : buffer_list_) {
      DeleteVao(elem.first);
    }
  }

  void BindVao(int vao) {
    glBindVertexArray(vao);
    bound_vao_ = vao;
  }
  void UnbindVao() {
    glBindVertexArray(0);
    bound_vao_ = 0;
  }

  void AddBuffer(int buffer_id) {
    if (bound_vao_ != 0)
      buffer_list_[bound_vao_].push_back(buffer_id);
    else
      throw;
  }

  void DeleteVao(int vao) {
    if (buffer_list_.find(vao) != buffer_list_.end()) {
      std::vector<int> buffers = buffer_list_[vao];
      glDeleteBuffers(buffers.size(), (GLuint*)&buffers[0]);
    }
  }

  bool IsVaoBound() { return bound_vao_; }

 private:
  int bound_vao_ = 0;
  std::map<int, std::vector<int> > buffer_list_;
};

}  // namespace quasi_game_engine

#endif  // QGE_OPENGL_BUFFER_MANAGER_H
