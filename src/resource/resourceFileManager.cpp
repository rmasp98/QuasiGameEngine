#include "resource/resourceFileManager.hpp"
#include "resource/texture.hpp"
#include <iostream>
#include <FreeImage.h>


namespace xxx {

   ResourceFileManager::ResourceFileManager() {

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
      if (compareFileType(fileLocation, freeimgExtensions)) //need to make this case insensitive
         return loadImage(fileLocation);
      else if (compareFileType(fileLocation, assimpExtensions))
         return loadObject(fileLocation);


      //assert that this file type is not supported
      std::cout << "The " << fileLocation << " file type is not supported" << '\n';
      return NULL;
   }




   std::string ResourceFileManager::getExtension(std::string fileLoction) {
      std::cout << "getting extension" << '\n';

      size_t i = fileLoction.rfind('.', fileLoction.length());
      if (i != std::string::npos) {
         return fileLoction.substr(i+1, fileLoction.length() - i);
      }

      //assert that file does not have extension
      std::cout << fileLoction << " does not have an extension. Cannot determine file type" << std::endl;
      return NULL;
   }



   bool ResourceFileManager::compareFileType(std::string fileLoction,
                                             std::vector<std::string> extensions) {

      std::cout << "Finding loader for this file type" << std::endl;

      std::string ext = getExtension(fileLoction);
      for (uint iExt=0; iExt < extensions.size(); ++iExt) {
         if (!ext.compare(extensions[iExt]))
            return true;
      }

      return false;

   }




   Resource* ResourceFileManager::loadImage(std::string fileLocation) {
      std::cout << "loading image" << '\n';

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

      std::cout << "Failed to load '" << fileLocation << "'" <<  std::endl;
      return NULL;
   }




   Resource* ResourceFileManager::loadObject(std::string fileLocation) {
      std::cout << "loading object" << '\n';

      return NULL;
   }

}
