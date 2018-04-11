#include "resource/texture.hpp"

#include "renderer/renderManager.hpp"
#include "utils/logging/logger.hpp"



namespace qge {

   Texture::Texture(byte* pixelMapIn, uint widthIn, uint heightIn)
                  : pixelMap(pixelMapIn), width(widthIn), height(heightIn) {}





   Texture::~Texture() {

   }


   void Texture::loadToGraphics(RenderManager* renderMan) {

      renderMan->loadTexture(*this);

   }

} // namespave qge
