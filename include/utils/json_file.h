/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_JSON_FILE_MANAGER_H
#define QGE_JSON_FILE_MANAGER_H

#include "utils/file_utils.h"

#include "nlohmann/json.hpp"

namespace quasi_game_engine {

class JsonFile {
 public:
  JsonFile() = default;
  ~JsonFile() = default;

  //Currently don't need any of these so they have been deleted for the time being
  JsonFile(const JsonFile&) = delete;
  JsonFile& operator=(const JsonFile&) = delete;
  JsonFile(JsonFile&&) = delete;
  JsonFile& operator=(JsonFile&&) = delete;

  nlohmann::json LoadFile(const char* file_name);
 private:
  nlohmann::json ParseFile(const char* file_name);
};

}  // namespace quasi_game_engine

#endif  // QGE_JSON_FILE_MANAGER_H
