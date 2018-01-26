#include "resource/resourceFileManager.hpp"
#include "resource/texture.hpp"
#include "utils/logger.hpp"

#include <FreeImage.h>


namespace xxx {

   ResourceFileManager::ResourceFileManager(Logger* logIn) {

      FileManager::setLogger(logIn);

      // True means extensions disabled
      FreeImage_Initialise(true);

      freeimgExtensions.push_back("dds");
      freeimgExtensions.push_back("jpg");
      assimpExtensions.push_back("obj");

   }


   ResourceFileManager::~ResourceFileManager() {
      FreeImage_DeInitialise();
   }

   Resource* ResourceFileManager::loadFile(std::string fileLocation) {

      //determine what the file is and then call the relevant function
      if (FileManager::compareFileType(fileLocation, freeimgExtensions)) //need to make this case insensitive
         return loadImage(fileLocation);
      else if (FileManager::compareFileType(fileLocation, assimpExtensions))
         return loadObject(fileLocation);


      //assert that this file type is not supported
      logger->log("The " + fileLocation + " file type is not supported", LOG_ERROR);
      return NULL;
   }



   Resource* ResourceFileManager::loadImage(std::string fileLocation) {
      logger->log("Loading image from " + fileLocation, LOG_TRACE);

      FREE_IMAGE_FORMAT filetype = FreeImage_GetFileType(fileLocation.c_str(), 0);
      if (filetype == FIF_UNKNOWN)
         filetype = FreeImage_GetFIFFromFilename(fileLocation.c_str());

      if ((filetype != FIF_UNKNOWN) && (FreeImage_FIFSupportsReading(filetype))) {

         FIBITMAP* dib = FreeImage_Load(filetype, fileLocation.c_str(), 0);
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

      logger->log("Failed to load " + fileLocation +
                  ". Either FreeImage does not support this filetype" +
                  ", the file is corrupt or does not exist",
                  LOG_ERROR);

      return NULL;
   }




   Resource* ResourceFileManager::loadObject(std::string fileLocation) {
      logger->log("Loading object from " + fileLocation, LOG_TRACE);

      return NULL;
   }

}
