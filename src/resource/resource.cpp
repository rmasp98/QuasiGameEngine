#include "resource/resource.hpp"
#include "renderer/renderManager.hpp"



#include <iostream>





namespace xxx {

   Resource::Resource() : renderMan(NULL) {}

   Resource::~Resource() {}

   void Resource::loadToGraphics() { std::cout << "This resource can't load to graphics" << '\n';};





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
