#ifndef XXX_TEXTURE_HPP
#define XXX_TEXTURE_HPP


#include "resource/resource.hpp"


typedef unsigned int uint;
typedef unsigned char byte;

namespace xxx {
   class RenderManager;

   class Texture : public Resource {
   public:
      Texture(byte* pixelMapIn, uint widthIn, uint heightIn);
      ~Texture();

      //Could include functions such as decompress, (un)load to graphics memory
      //gamma stuff,

      uint getResource() { return textureID.get(); };

      void loadToGraphics(RenderManager* renderMan);

      data<byte*> pixelMap;
      data<uint> width, height, depth, textureID;

   private:
      //use data class to define config of image
      // built in mipmap, number of mipmap levels
      // filters (enum) - linear, bilinear, etc
      // texture wrapping
      // texture type (enum) - 2d, 3d, cubemap, etc.
      // pixel type (enum) - RGB, RGBA, etc.
      // bool compressed

   };
}


#endif // XXX_TEXTURE_HPP
