#include "resource/resourceFileManager.hpp"
#include "resource/texture.hpp"
#include "utils/logging/logger.hpp"

#include <FreeImage.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>


namespace qge {

   ResourceFileManager::ResourceFileManager(Logger* logIn) {

      FileManager::setLogger(logIn);

      // True means extensions disabled
      FreeImage_Initialise(true);

      freeimgExtensions.push_back("dds");
      freeimgExtensions.push_back("jpg");
      assimpExtensions.push_back("obj");
      assimpExtensions.push_back("3ds");

   }


   ResourceFileManager::~ResourceFileManager() {
      FreeImage_DeInitialise();
   }




   uint ResourceFileManager::calcGUID(std::string fileLocation,
                                     std::string assetName) {
      //do stuff
      return 0;
   }






   Resource* ResourceFileManager::loadFile(Asset asset) {

      //determine what the file is and then call the relevant function
      if (FileManager::compareFileType(asset.fileLocation, freeimgExtensions)) //need to make this case insensitive
         return loadImage(asset);
      else if (FileManager::compareFileType(asset.fileLocation, assimpExtensions))
         return loadObject(asset);


      //assert that this file type is not supported
      LOG(LOG_ERROR, logger) << "The " << asset.fileLocation
                             << " file type is not supported";
      return NULL;
   }



   Resource* ResourceFileManager::loadImage(Asset asset) {
      LOG(LOG_TRACE, logger) << "Loading image from '" << asset.fileLocation << "'";

      FREE_IMAGE_FORMAT filetype = FreeImage_GetFileType(asset.fileLocation.c_str(), 0);
      if (filetype == FIF_UNKNOWN)
         filetype = FreeImage_GetFIFFromFilename(asset.fileLocation.c_str());

      if ((filetype != FIF_UNKNOWN) && (FreeImage_FIFSupportsReading(filetype))) {

         FIBITMAP* dib = FreeImage_Load(filetype, asset.fileLocation.c_str(), 0);
         if (dib) {
            uint bitsPerPixel = FreeImage_GetBPP(dib);

            // Convert to 32 bit pixelMap
            FIBITMAP* dib32;
            if (bitsPerPixel == 32)
               dib32 = dib;
            else
               dib32 = FreeImage_ConvertTo32Bits(dib);

            byte* pixelMap = FreeImage_GetBits(dib32);
            uint width = FreeImage_GetWidth(dib32);
            uint height = FreeImage_GetHeight(dib32);

            Resource* newResource = new Texture(pixelMap, width, height);
            return newResource;
         }

      }

      LOG(LOG_ERROR, logger) << "Failed to load " << asset.fileLocation <<
                  ". Either FreeImage does not support this filetype" <<
                  ", the file is corrupt or does not exist";

      return NULL;
   }




   Resource* ResourceFileManager::loadObject(Asset asset) {
      //May need to start the assimp logger to get more information out
      LOG(LOG_TRACE, logger) << "Loading object from " << asset.fileLocation;

      Assimp::Importer importer;

      // The postprocessing will need to be set up in the resource manager beforehand
      const aiScene* scene = importer.ReadFile(asset.fileLocation,
                     aiProcess_CalcTangentSpace        |
                     aiProcess_Triangulate             |
                     aiProcess_JoinIdenticalVertices   |
                     aiProcess_SortByPType
      );

      if (scene) {

         for (uint iMesh=0; iMesh < scene->mNumMeshes; ++iMesh) {
            const aiMesh* mesh = scene->mMeshes[iMesh];
            if (calcGUID(asset.fileLocation, mesh->mName.C_Str()) == asset.guid) {
               LOG(LOG_DEBUG, logger) << mesh->mName.C_Str();

               float *verticies = NULL, *normals = NULL, *textureCoords = NULL;
               uint* faces;

               if (mesh->HasFaces()) {
                  LOG(LOG_TRACE, logger) << "Getting faces from '" << asset.assetName <<
                              "', in '" << asset.fileLocation << "'";

                  faces = new uint[mesh->mNumFaces * mesh->mFaces[0].mNumIndices];
                  uint faceIndex = 0;
                  for (uint iFace=0; iFace < mesh->mNumFaces; ++iFace) {
                     const aiFace* face = &mesh->mFaces[iFace];
                     for (uint jFace=0; jFace < face->mNumIndices; ++jFace) {
                        faces[faceIndex + jFace] = face->mIndices[jFace];
                     }
                     faceIndex += face->mNumIndices;
                  }
               }


               if (mesh->HasPositions()) {
                  LOG(LOG_TRACE, logger) << "Getting verticies from '" << asset.assetName <<
                              "', in '" << asset.fileLocation << "'";

                  verticies = new float[mesh->mNumVertices * 3];
                  uint vertIndex = 0;
                  for (uint iVert=0; iVert < mesh->mNumVertices; ++iVert) {
                     const aiVector3D* vert = &mesh->mVertices[iVert];
                     for (uint jVert=0; jVert < 3 ; ++jVert) {
                        verticies[vertIndex + jVert] = vert->operator[](jVert);
                     }
                     vertIndex += 3;
                  }
               }

               if (mesh->HasNormals()) {
                  LOG(LOG_TRACE, logger) << "Getting normals from '" << asset.assetName <<
                              "', in '" << asset.fileLocation << "'";

                  normals = new float[mesh->mNumVertices * 3];
                  uint normIndex = 0;
                  for (uint iNorm=0; iNorm < mesh->mNumVertices; ++iNorm) {
                     const aiVector3D* normal = &mesh->mNormals[iNorm];
                     for (uint jNorm=0; jNorm < 3 ; ++jNorm) {
                        normals[normIndex + jNorm] = normal->operator[](jNorm);
                     }
                     normIndex += 3;
                  }
               }

               if (mesh->HasTextureCoords(0)) {
                  LOG(LOG_TRACE, logger) << "Getting texture coordinates from '" <<
                              asset.assetName << "', in '" << asset.fileLocation << "'";

                  uint uvType = *mesh->mNumUVComponents;
                  textureCoords = new float[mesh->mNumVertices * uvType];
                  uint uvIndex = 0;
                  for (uint iUV=0; iUV < mesh->mNumVertices; ++iUV) {
                     const aiVector3D* uv = &mesh->mTextureCoords[0][iUV];
                     for (uint jUV=0; jUV < uvType ; ++jUV) {
                        textureCoords[uvIndex + jUV] = uv->operator[](jUV);
                     }
                     uvIndex += uvType;
                  }
               }


               // Still can get out bones, animations?, tangents, bitangents
               // colours, material, method?, primitive type

               Resource* newResource = new Mesh(verticies,
                                                normals,
                                                textureCoords,
                                                faces,
                                                mesh->mNumVertices,
                                                mesh->mNumFaces);

               return newResource;

            }
         }

      }



      LOG(LOG_TRACE, logger) << "Failed to load " << asset.fileLocation <<
                  ". Either AssImp does not support this filetype" <<
                  ", the file is corrupt or does not exist";

      return NULL;

   }

}  // namespave qge
