#ifndef QGE_JSON_FILE_MANAGER_HPP
#define QGE_JSON_FILE_MANAGER_HPP

#include "utils/fileManager.hpp"
#include "utils/jsonFileManager/jsonFile.hpp"


namespace qge {
   class JsonFileManager : public FileManager {
   public:
      JsonFileManager(Logger* logIn);
      ~JsonFileManager();

      JsonFile* loadFile(const char* newFileName = NULL);

   private:
      JsonFile* file;
      const char* fileName;

   };

} // namespave qge


#endif // QGE_JSON_FILE_MANAGER_HPP
