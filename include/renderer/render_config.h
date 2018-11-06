/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_RENDER_CONFIG_H
#define QGE_RENDER_CONFIG_H

namespace quasi_game_engine {

// Indicates default settings for rendering scenes
enum RenderConfigType { RC_IMGUI, RC_DEFAULT };

// need to redo this later to make it better
class RenderConfig {
  /*------------------------------------------------------------------------------
    Interface class for all graphic config. Should be able o handle
  configuration for all supported graphics APIs Notes
  - Notes go here
  ------------------------------------------------------------------------------*/
 public:
  virtual ~RenderConfig() = default;

  //Getting rid of copy/move constructors/assignment operators (may need later)
  RenderConfig(const RenderConfig&) = delete;
  RenderConfig& operator=(const RenderConfig&) = delete;
  RenderConfig(RenderConfig&&) = delete;
  RenderConfig& operator=(RenderConfig&&) = delete;

  virtual void SetCullFace(bool is_enabled) = 0;
  virtual void SetDepthTest(bool is_enabled) = 0;
  virtual void SetScissorTest(bool is_enabled) = 0;
  virtual void SetBlend(int src_func, int dest_func, int equation) = 0;
  virtual void UnsetBlend() = 0;
  virtual void SetPolygonMode(int face_enum, int mode_enum) = 0;
  virtual void ApplyConfig() = 0;

 protected:
  RenderConfig() = default;
};

}  // namespace quasi_game_engine

#endif  // QGE_RENDER_CONFIG_H
