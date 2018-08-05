#include "resource/resource.h"


#include "renderer/renderer.h"
#include "utils/logging/logger.h"


#include <cassert>





namespace quasi_game_engine {

   Logger* Resource::logger_;

   Resource::Resource(Asset asset) : asset_(asset) {
      assert(Resource::logger_ != NULL);
   }

   Resource::~Resource() {}

   void Resource::LoadToGraphics(Renderer* render_manager) {
      LOG(LOG_ERROR, logger_) << "This resource cannot be loaded to graphics!";
   }






   // Model::Model() {
   //
   // }
   //
   // Model::~Model() {
   //
   // }





   // Need to benchmark if this or just passing a reference would be quicker
   Mesh::Mesh(float* verts, float* norms,
              float* uvs, int* faces,
              int num_verts, int num_faces,
              Asset asset)
            : Resource(asset), num_verts_(num_verts), num_faces_(num_faces) {

      verts_ = new float[num_verts_ * 3];
      std::copy(verts, verts + (num_verts_ * 3), verts_);

      norms_ = new float[num_verts_ * 3];
      std::copy(norms, norms + (num_verts_ * 3), norms_);

      uvs_ = new float[num_verts_ * 3];
      std::copy(uvs, uvs + (num_verts_ * 3), uvs_);

      faces_ = new int[num_faces_ * 3];
      std::copy(faces, faces + (num_faces_ * 3), faces_);

      vao_ = 0;

   }


   Mesh::~Mesh() {
      delete verts_;
      delete norms_;
      delete uvs_;
      delete faces_;
   }


   void Mesh::GetMesh(float** verts, float** norms,
                      float** uvs, int** faces,
                      int &num_verts, int &num_faces) {

      *verts = verts_;
      *norms = norms_;
      *uvs   = uvs_;
      *faces = faces_;
      num_verts = num_verts_;
      num_faces = num_faces_;
   }

   void Mesh::LoadToGraphics(Renderer* render_manager) {
      //renMan->loadObject(*this);

      LOG(LOG_INFO, logger_) << "Loading mesh for '" << asset_.asset_name
                            << "' in '" << asset_.file_location << "' to graphics";

      //check if indices have been generated

      LOG(LOG_INFO, logger_) << "Loading verticies to graphics";
      render_manager->LoadVertexAttribute(verts_, num_verts_,
                                          Attribute(0, 3), &vao_);

      LOG(LOG_INFO, logger_) << "Loading normals to graphics";
      render_manager->LoadVertexAttribute(norms_, num_verts_,
                                          Attribute(1, 3), &vao_);

      LOG(LOG_INFO, logger_) << "Loading texture coordinates to graphics";
      render_manager->LoadVertexAttribute(uvs_, num_verts_,
                                          Attribute(2, 2), &vao_);

      LOG(LOG_INFO, logger_) << "Loading VBO indices to graphics";
      render_manager->LoadVertexIndices(faces_, num_faces_, 3, &vao_);
   }




   // Material::Material() {
   //
   // }
   //
   // Material::~Material() {
   //
   // }

} // namespace quasi_game_engine
