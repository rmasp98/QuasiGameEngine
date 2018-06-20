#ifndef QGE_FILE_MANAGER_H
#define QGE_FILE_MANAGER_H

#include <string>
#include <vector>
#include <fstream>



namespace quasi_game_engine {
   class Logger;

   class FileManager {
   public:
      virtual ~FileManager() {};

      void SetLogger(Logger* logger) { logger_ = logger; };

   protected:
      Logger* logger_;

      FileManager() {};
      std::string GetExtension(std::string file_loction);
      bool CompareFileType(std::string file_loction, std::vector<std::string> extensions);
   };


} // namespace quasi_game_engine


#endif // QGE_FILE_MANAGER_H
