#ifndef QGE_RENDER_CONFIG_H
#define QGE_RENDER_CONFIG_H

namespace quasi_game_engine {

enum RenderConfigType {
  RC_IMGUI, RC_DEFAULT
};

// need to redo this later to make it better
class RenderConfig {
/*Notes-------------------------------------------------------------------------
- Notes go here
------------------------------------------------------------------------------*/
 public:
  virtual ~RenderConfig() = default;

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

} // namespace quasi_game_engine

#endif //QGE_RENDER_CONFIG_H
