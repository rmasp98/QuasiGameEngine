/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_JSON_FILE_MANAGER_H
#define QGE_JSON_FILE_MANAGER_H

#include "utils/file_manager.h"

#include "nlohmann/json.hpp"

namespace quasi_game_engine {

class JsonFileManager : public FileManager {
 public:
  explicit JsonFileManager(Logger* logger);
  ~JsonFileManager() = default;

  //JsonFile LoadFile(const char* file_name);
  nlohmann::json LoadFile(const char* file_name);
 private:
  nlohmann::json ParseFile(const char* config_file_name);
  Logger logger_;
};

}  // namespace quasi_game_engine

#endif  // QGE_JSON_FILE_MANAGER_H
