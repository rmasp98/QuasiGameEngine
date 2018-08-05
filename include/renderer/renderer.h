#ifndef QGE_RENDERER_H
#define QGE_RENDERER_H

typedef unsigned int uint;

namespace quasi_game_engine {
class Logger;
class LogWorker;


//Need to update these types to make universal
struct Attribute {
  Attribute(int index_in, int components_in)
    : attribute_index(index_in), num_components(components_in) {};

  int attribute_index;
  int num_components;
  //GL_TYPE dataType;
  //GL_BOOL normalised;
};

/*Notes----------------------------------------------------------------------
- The should be a parent class with child classes for opengl and directx
---------------------------------------------------------------------------*/
class Renderer {
 public:
  virtual ~Renderer() = default;

  //Getting rid of copy/move constructors/assignment operators (may need later)
  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;
  Renderer(Renderer&&) = delete;
  Renderer& operator=(Renderer&&) = delete;

  virtual bool InitGraphics() = 0;
  virtual void Draw() = 0;

  virtual bool LoadImage(const unsigned char *pixel_map, int width, int height,
                         bool is_mipmap, uint* texture_id) = 0;

  virtual bool LoadVertexAttribute(const float* attribute_data, int num_verts,
                                   Attribute attribute, uint* vao) = 0;

  virtual bool LoadVertexIndices(const int *indices, int num_indices,
                                 int num_components, uint* vao) = 0;

 protected:
  Renderer() = default;
};

} // namespace quasi_game_engine

#endif // QGE_RENDERER_H
