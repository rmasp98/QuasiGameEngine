#ifndef QGE_TEXTURE_H
#define QGE_TEXTURE_H


#include "resource/resource.h"

namespace quasi_game_engine {
   class Renderer;

   class Texture : public Resource {
   public:
      Texture(unsigned char* pixel_map, int width,
              int height, Asset asset);
      ~Texture();

      //Could include functions such as decompress, (un)load to graphics memory
      //gamma stuff,

      //should probably check that it has been loaded to graphics
      uint GetResource() { return texture_id_; };

      virtual void LoadToGraphics(Renderer* render_manager);

      //data<byte*> pixelMap;
      //data<uint> width, height, depth, textureID;

   private:
      unsigned char *pixel_map_;
      int width_, height_, depth_;
      uint texture_id_;

      //use data class to define config of image
      // built in mipmap, number of mipmap levels
      // filters (enum) - linear, bilinear, etc
      // texture wrapping
      // texture type (enum) - 2d, 3d, cubemap, etc.
      // pixel type (enum) - RGB, RGBA, etc.
      // bool compressed

   };
} // namespace quasi_game_engine


#endif // QGE_TEXTURE_H
