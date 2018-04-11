#ifndef QGE_RESOURCE_FILE_MANAGER_HPP
#define QGE_RESOURCE_FILE_MANAGER_HPP

#include "utils/fileManager.hpp"

#include <string>
#include <vector>



namespace qge {
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
} // namespave qge


#endif // QGE_RESOURCE_FILE_MANAGER_HPP
