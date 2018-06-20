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

   }

} // namespace quasi_game_engine

#endif // QGE_JSON_UTILS_H
