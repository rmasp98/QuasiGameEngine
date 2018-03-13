#ifndef XXX_RESOURCE_FILE_MANAGER_HPP
#define XXX_RESOURCE_FILE_MANAGER_HPP

#include "utils/fileManager.hpp"

#include <string>
#include <vector>



namespace xxx {
   class Resource;
   class Logger;
   struct Asset;

   class ResourceFileManager : public FileManager {
   public:
      ResourceFileManager(Logger* logIn);
      ~ResourceFileManager();

      Resource* loadFile(Asset asset);
      uint calcGUID(std::string fileLocation, std::string assetName);

   protected:
      std::vector<std::string> freeimgExtensions, assimpExtensions;

      Resource* loadImage(Asset asset);
      Resource* loadObject(Asset asset);

   };
}


#endif // XXX_RESOURCE_FILE_MANAGER_HPP
