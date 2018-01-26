#ifndef XXX_RESOURCE_FILE_MANAGER_HPP
#define XXX_RESOURCE_FILE_MANAGER_HPP

#include "utils/fileManager.hpp"

#include <string>
#include <vector>



namespace xxx {
   class Resource;
   class Logger;

   class ResourceFileManager : public FileManager {
   public:
      ResourceFileManager(Logger* logIn);
      ~ResourceFileManager();

      Resource* loadFile(std::string fileLocation);


   protected:


      std::vector<std::string> freeimgExtensions, assimpExtensions;

      std::string getExtension(std::string fileLoction);
      bool compareFileType(std::string fileLoction, std::vector<std::string> extensions);

      Resource* loadImage(std::string fileLocation);
      Resource* loadObject(std::string fileLocation);
   };
}


#endif // XXX_RESOURCE_FILE_MANAGER_HPP
