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
      LOG(LOG_ERROR, logger) << "This resource can't load to graphics";
   }






   Model::Model() {

   }

   Model::~Model() {

   }





   // Need to benchmark if this or just passing a reference would be quicker
   Mesh::Mesh(float* vertsIn, float* normsIn,
              float* uvsIn, uint* facesIn,
              uint nVertsIn, uint nFacesIn)
            : nVerts(nVertsIn), nFaces(nFacesIn) {

      verts = new float[nVerts * 3];
      std::copy(vertsIn, vertsIn + (nVerts * 3), verts);

      norms = new float[nVerts * 3];
      std::copy(normsIn, normsIn + (nVerts * 3), norms);

      uvs = new float[nVerts * 3];
      std::copy(uvsIn, uvsIn + (nVerts * 3), uvs);

      faces = new uint[nFaces * 3];
      std::copy(facesIn, facesIn + (nFaces * 3), faces);
      printf("%i\n", nFaces);

   }


   Mesh::~Mesh() {
      delete verts;
      delete norms;
      delete uvs;
      delete faces;
   }

   //THis should eventually just put this on a queue
   void Mesh::loadToGraphics(RenderManager* renderMan) {
      renderMan->loadObject(*this);
   }


   void Mesh::getMesh(float** vertsOut, float** normsOut,
                      float** uvsOut, uint** facesOut,
                      uint &nVertsOut, uint &nFacesOut) {

      *vertsOut = verts;
      *normsOut = norms;
      *uvsOut   = uvs;
      *facesOut = faces;
      nVertsOut = nVerts;
      nFacesOut = nFaces;
   }




   Material::Material() {

   }

   Material::~Material() {

   }










}
