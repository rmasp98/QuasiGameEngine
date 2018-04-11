#ifndef QGE_JSON_UTILS_HPP
#define QGE_JSON_UTILS_HPP

#include "utils/jsonFileManager/rapidjson/document.h"
#include "utils/jsonFileManager/rapidjson/istreamwrapper.h"
namespace rj = rapidjson;

namespace qge {
   class Logger;

   namespace json {
      // could make it return a bool whether passed or not
      template <typename TYPE>
      void getKey(rj::Value* keyIn, TYPE& keyOut, Logger* logger);

   }

} // ns qge

#endif // QGE_JSON_UTILS_HPP
