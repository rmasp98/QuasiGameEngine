/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_JSON_FILE_H
#define QGE_JSON_FILE_H

#include "utils/jsonFileManager/json_array.h"
#include "utils/jsonFileManager/json_utils.h"
#include "utils/logging/logger.h"

#include "utils/jsonFileManager/rapidjson/document.h"
namespace rj = rapidjson;

namespace quasi_game_engine {

class JsonFile {
  friend class JsonFileManager;

 public:
  ~JsonFile() = default;

   //Getting rid of copy/move constructors/assignment operators (may need later)
  JsonFile(const JsonFile& out) = default;
  JsonFile& operator=(const JsonFile&) = default;
  JsonFile(JsonFile&&) = default;
  JsonFile& operator=(JsonFile&&) = default;

  template <typename TYPE>
  TYPE GetKey(const char* new_key, bool must_exist = false) {
    rj::Value* new_object = ExtractKey(new_key);

    TYPE key_out{};  //{} initialises it
    if (new_object != nullptr)
      json::GetKey(new_object, key_out, logger_);
    else if (must_exist) {
      LOG(LOG_ERROR, logger_) << new_key << " does not exist";
      throw "";
    }

    return key_out;
  }

  bool CheckKey(const char* new_key) { return doc_->HasMember(new_key); }

 protected:
  JsonFile(Logger* logger) : logger_(logger){};
  bool ParseFile(const char* config_file_name);

 private:
  rj::Document* doc_;
  Logger* logger_;

  rj::Value* ExtractKey(const char* new_key);
};

}  // namespace quasi_game_engine

#endif  // QGE_JSON_FILE_H
