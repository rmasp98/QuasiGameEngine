#include "resource/texture.hpp"
#include "renderer/renderManager.hpp"




#include <iostream>


namespace xxx {

   Texture::Texture(byte* pixelMapIn, uint widthIn, uint heightIn)
                  : pixelMap(pixelMapIn), width(widthIn), height(heightIn) {}





   Texture::~Texture() {

   }


   void Texture::loadToGraphics() {
      std::cout << "test" << '\n';
      renderMan->loadTexture(*this);

   }

}
