#include "resource/resource.hpp"


#include "renderer/renderManager.hpp"
#include "utils/logger.hpp"


#include <cassert>





namespace xxx {

   Logger* Resource::logger;

   Resource::Resource() {
      assert(Resource::logger != NULL);
   }

   Resource::~Resource() {}

   void Resource::loadToGraphics(RenderManager* renderMan = NULL) {
      logger->log("This resource can't load to graphics", LOG_ERROR);
   }






   Model::Model() {

   }

   Model::~Model() {

   }






   Mesh::Mesh() {

   }

   Mesh::~Mesh() {

   }




   Material::Material() {

   }

   Material::~Material() {

   }










}
