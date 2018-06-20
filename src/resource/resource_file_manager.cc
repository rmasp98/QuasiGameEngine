#include "resource/resource_file_manager.h"
#include "resource/texture.h"
#include "utils/logging/logger.h"

#include <FreeImage.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>


namespace quasi_game_engine {

   ResourceFileManager::ResourceFileManager(Logger* logIn) {

      FileManager::SetLogger(logIn);

      // True means extensions disabled
      FreeImage_Initialise(true);

      freeimg_extensions_.push_back("dds");
      freeimg_extensions_.push_back("jpg");
      assimp_extensions_.push_back("obj");
      assimp_extensions_.push_back("3ds");

   }


   ResourceFileManager::~ResourceFileManager() {
      FreeImage_DeInitialise();
   }




   uint ResourceFileManager::CalcGUID(std::string file_location,
                                      std::string asset_name) {
      //do stuff
      return 0;
   }






   Resource* ResourceFileManager::LoadFile(Asset asset) {

      //determine what the file is and then call the relevant function
      if (FileManager::CompareFileType(asset.file_location, freeimg_extensions_))
         return LoadImage(asset);
      else if (FileManager::CompareFileType(asset.file_location, assimp_extensions_))
         return LoadObject(asset);


      //assert that this file type is not supported
      LOG(LOG_ERROR, logger_) << "The " << asset.file_location
                             << " file type is not supported";
      return NULL;
   }



   Resource* ResourceFileManager::LoadImage(Asset asset) {
      LOG(LOG_TRACE, logger_) << "Loading image from '" << asset.file_location << "'";

      FREE_IMAGE_FORMAT file_type = FreeImage_GetFileType(asset.file_location.c_str(), 0);
      if (file_type == FIF_UNKNOWN)
         file_type = FreeImage_GetFIFFromFilename(asset.file_location.c_str());

      if ((file_type != FIF_UNKNOWN) && (FreeImage_FIFSupportsReading(file_type))) {

         FIBITMAP* dib = FreeImage_Load(file_type, asset.file_location.c_str(), 0);
         if (dib) {
            int bits_per_pixel = FreeImage_GetBPP(dib);

            // Convert to 32 bit pixelMap
            FIBITMAP *dib32;
            if (bits_per_pixel == 32)
               dib32 = dib;
            else
               dib32 = FreeImage_ConvertTo32Bits(dib);

            unsigned char *pixel_map = FreeImage_GetBits(dib32);
            int width = FreeImage_GetWidth(dib32);
            int height = FreeImage_GetHeight(dib32);

            Resource* new_resource = new Texture(pixel_map, width, height, asset);
            return new_resource;
         }

      }

      LOG(LOG_ERROR, logger_) << "Failed to load " << asset.file_location <<
                  ". Either FreeImage does not support this filetype" <<
                  ", the file is corrupt or does not exist";

      return NULL;
   }




   Resource* ResourceFileManager::LoadObject(Asset asset) {
      //May need to start the assimp logger to get more information out
      LOG(LOG_TRACE, logger_) << "Loading object from " << asset.file_location;

      Assimp::Importer importer;

      // The postprocessing will need to be set up in the resource manager beforehand
      const aiScene* scene = importer.ReadFile(asset.file_location,
                     aiProcess_CalcTangentSpace        |
                     aiProcess_Triangulate             |
                     aiProcess_JoinIdenticalVertices   |
                     aiProcess_SortByPType
      );

      if (scene) {

         for (size_t i_mesh=0; i_mesh < scene->mNumMeshes; ++i_mesh) {
            const aiMesh* mesh = scene->mMeshes[i_mesh];
            if (CalcGUID(asset.file_location, mesh->mName.C_Str()) == asset.guid) {
               LOG(LOG_DEBUG, logger_) << mesh->mName.C_Str();

               float *verticies = NULL, *normals = NULL, *texture_coords = NULL;
               int *faces;

               if (mesh->HasFaces()) {
                  LOG(LOG_TRACE, logger_) << "Getting faces from '" << asset.asset_name <<
                              "', in '" << asset.file_location << "'";

                  faces = new int[mesh->mNumFaces * mesh->mFaces[0].mNumIndices];
                  int face_index = 0;
                  for (size_t i_face=0; i_face < mesh->mNumFaces; ++i_face) {
                     const aiFace* face = &mesh->mFaces[i_face];
                     for (size_t j_face=0; j_face < face->mNumIndices; ++j_face) {
                        faces[face_index + j_face] = face->mIndices[j_face];
                     }
                     face_index += face->mNumIndices;
                  }
               }


               if (mesh->HasPositions()) {
                  LOG(LOG_TRACE, logger_) << "Getting verticies from '" << asset.asset_name <<
                              "', in '" << asset.file_location << "'";

                  verticies = new float[mesh->mNumVertices * 3];
                  int vert_index = 0;
                  for (size_t i_vert=0; i_vert < mesh->mNumVertices; ++i_vert) {
                     const aiVector3D* vert = &mesh->mVertices[i_vert];
                     for (size_t j_vert=0; j_vert < 3 ; ++j_vert) {
                        verticies[vert_index + j_vert] = (*vert)[j_vert];
                     }
                     vert_index += 3;
                  }
               }

               if (mesh->HasNormals()) {
                  LOG(LOG_TRACE, logger_) << "Getting normals from '" << asset.asset_name <<
                              "', in '" << asset.file_location << "'";

                  normals = new float[mesh->mNumVertices * 3];
                  int norm_index = 0;
                  for (size_t i_norm=0; i_norm < mesh->mNumVertices; ++i_norm) {
                     const aiVector3D* normal = &mesh->mNormals[i_norm];
                     for (size_t j_norm=0; j_norm < 3 ; ++j_norm) {
                        normals[norm_index + j_norm] = (*normal)[j_norm];
                     }
                     norm_index += 3;
                  }
               }

               if (mesh->HasTextureCoords(0)) {
                  LOG(LOG_TRACE, logger_) << "Getting texture coordinates from '" <<
                              asset.asset_name << "', in '" << asset.file_location << "'";

                  int uv_type = *mesh->mNumUVComponents;
                  texture_coords = new float[mesh->mNumVertices * uv_type];
                  int uv_index = 0;
                  for (size_t i_uv=0; i_uv < mesh->mNumVertices; ++i_uv) {
                     const aiVector3D* uv = &mesh->mTextureCoords[0][i_uv];
                     for (size_t j_uv=0; j_uv < (unsigned)uv_type ; ++j_uv) {
                        texture_coords[uv_index + j_uv] = (*uv)[j_uv];
                     }
                     uv_index += uv_type;
                  }
               }


               // Still can get out bones, animations?, tangents, bitangents
               // colours, material, method?, primitive type

               Resource* new_resource = new Mesh(verticies,
                                                 normals,
                                                 texture_coords,
                                                 faces,
                                                 mesh->mNumVertices,
                                                 mesh->mNumFaces,
                                                 asset);

               return new_resource;

            }
         }

      }



      LOG(LOG_TRACE, logger_) << "Failed to load " << asset.file_location <<
                  ". Either AssImp does not support this filetype" <<
                  ", the file is corrupt or does not exist";

      return NULL;

   }

}  // namespace quasi_game_engine
