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
#include "utils/jsonFileManager/json_file.h"

namespace quasi_game_engine {
class JsonFileManager : public FileManager {
 public:
  explicit JsonFileManager(Logger* logger);
  ~JsonFileManager() = default;

  JsonFile LoadFile(const char* file_name);
};

}  // namespace quasi_game_engine

#endif  // QGE_JSON_FILE_MANAGER_H
