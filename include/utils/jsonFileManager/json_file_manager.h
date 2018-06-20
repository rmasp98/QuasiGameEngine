#ifndef QGE_JSON_FILE_MANAGER_H
#define QGE_JSON_FILE_MANAGER_H

#include "utils/file_manager.h"
#include "utils/jsonFileManager/json_file.h"


namespace quasi_game_engine {
   class JsonFileManager : public FileManager {
   public:
      JsonFileManager(Logger* logger);
      ~JsonFileManager();

      JsonFile* LoadFile(const char* new_file_name = NULL);

   private:
      JsonFile file_;
      const char* file_name_;

   };

} // namespace quasi_game_engine


#endif // QGE_JSON_FILE_MANAGER_H
