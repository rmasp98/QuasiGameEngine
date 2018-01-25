#ifndef XXX_FILE_MANAGER_HPP
#define XXX_FILE_MANAGER_HPP

#include "resource/resource.hpp"
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GL/gl.h>


namespace xxx {
   class ResourceFileManager {
   public:
      ResourceFileManager();
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


#endif // XXX_FILE_MANAGER_HPP
