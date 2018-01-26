#include "resource/texture.hpp"

#include "renderer/renderManager.hpp"
#include "utils/logger.hpp"



namespace xxx {

   Texture::Texture(byte* pixelMapIn, uint widthIn, uint heightIn)
                  : pixelMap(pixelMapIn), width(widthIn), height(heightIn) {}





   Texture::~Texture() {

   }


   void Texture::loadToGraphics(RenderManager* renderMan) {
      Resource::logger->log("Test", LOG_INFO);
      renderMan->loadTexture(*this);

   }

}
