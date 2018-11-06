/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_JSON_UTILS_H
#define QGE_JSON_UTILS_H

#include "utils/jsonFileManager/rapidjson/document.h"
#include "utils/jsonFileManager/rapidjson/istreamwrapper.h"
namespace rj = rapidjson;

namespace quasi_game_engine {
class Logger;

namespace json {
// could make it return a bool whether passed or not
template <typename TYPE>
void GetKey(rj::Value* key, TYPE& key_out, Logger* logger);

}  // namespace json

}  // namespace quasi_game_engine

#endif  // QGE_JSON_UTILS_H
