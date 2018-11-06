/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_JSON_ARRAY_H
#define QGE_JSON_ARRAY_H

#include "utils/jsonFileManager/json_file.h"
#include "utils/jsonFileManager/json_utils.h"
#include "utils/logging/logger.h"

#include "utils/jsonFileManager/rapidjson/document.h"
#include "utils/jsonFileManager/rapidjson/istreamwrapper.h"
namespace rj = rapidjson;

namespace quasi_game_engine {
class JsonArrayObject {
  friend class JsonFile;

 public:
  // Need to find a way to protect this constructor
  JsonArrayObject(rj::Value* array, Logger* logger)
      : array_(array), size_(array->Size()), logger_(logger){};
  ~JsonArrayObject(){};

  int GetSize() { return size_; };

  template <typename TYPE>
  TYPE GetKey(int element, const char* new_key) {
    if (element < size_) {
      rj::Value* new_object = ExtractKey(element, new_key);

      TYPE key_out;
      json::GetKey(new_object, key_out, logger_);

      return key_out;
    }

    LOG(LOG_ERROR, logger_)
        << "Json array does not have " << element << " elements";
    throw "";
  }

  bool CheckKey(int element, const char* new_key) {
    return (*array_)[element].HasMember(new_key);
  }

 private:
  rj::Value* array_;
  int size_;
  Logger* logger_;

  rj::Value* ExtractKey(int element, const char* new_key) {
    LOG(LOG_TRACE, logger_)
        << "Checking to see if '" << new_key << "' key exists";
    if ((*array_)[element].HasMember(new_key))
      return &((*array_)[element][new_key]);

    LOG(LOG_ERROR, logger_) << "Cannot find key " << new_key;
    throw "";
  }
};

}  // namespace quasi_game_engine

#endif  // QGE_JSON_ARRAY_H
