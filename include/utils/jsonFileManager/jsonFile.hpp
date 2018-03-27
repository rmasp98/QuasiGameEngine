#ifndef XXX_JSON_FILE_HPP
#define XXX_JSON_FILE_HPP

#include "utils/jsonFileManager/jsonArray.hpp"
#include "utils/jsonFileManager/jsonUtils.hpp"
#include "utils/logging/logger.hpp"

#include "utils/jsonFileManager/rapidjson/document.h"
namespace rj = rapidjson;

namespace xxx {
   class Logger;

   class JsonFile {
      friend class JsonFileManager;
   public:
      ~JsonFile() {};

      template<typename TYPE>
      TYPE getKey(const char* newKey, bool mustExist = false) {
         rj::Value* newObject = extractKey(newKey);

         TYPE keyOut{}; //{} initialises it
         if (newObject != nullptr)
            json::getKey(newObject, keyOut, logger);
         else if (mustExist) {
            LOG(LOG_ERROR, logger) << newKey << " does not exist";
            throw "";
         }

         return keyOut;
      }

      bool checkKey(const char* newKey) {
         return doc.HasMember(newKey);
      }

   protected:
      JsonFile(Logger* loggerIn) : logger(loggerIn) {};
      bool parseFile(const char* cfgFileName);

   private:
      rj::Document doc;
      Logger* logger;

      rj::Value* extractKey(const char* newKey);

   };
}



#endif // XXX_JSON_FILE_HPP
