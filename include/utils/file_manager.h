#ifndef QGE_FILE_MANAGER_H
#define QGE_FILE_MANAGER_H

#include "utils/qge_array.h"

#include <fstream>
#include <string>


namespace quasi_game_engine {
class Logger;

class FileManager {
 public:
  virtual ~FileManager(){};

  void SetLogger(Logger* logger) { logger_ = logger; };

 protected:
  Logger* logger_;

  FileManager(){};
  std::string GetExtension(std::string file_loction);
  bool CompareFileType(const char* file_loction,
                       QgeArray<const char*> extensions);
};

}  // namespace quasi_game_engine

#endif  // QGE_FILE_MANAGER_H
