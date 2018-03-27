#ifndef XXX_JSON_FILE_MANAGER_HPP
#define XXX_JSON_FILE_MANAGER_HPP

#include "utils/fileManager.hpp"
#include "utils/jsonFileManager/jsonFile.hpp"


namespace xxx {
   class JsonFileManager : public FileManager {
   public:
      JsonFileManager(Logger* logIn);
      ~JsonFileManager();

      JsonFile* loadFile(const char* newFileName = NULL);

   private:
      JsonFile* file;
      const char* fileName;

   };
}


#endif // XXX_JSON_FILE_MANAGER_HPP
