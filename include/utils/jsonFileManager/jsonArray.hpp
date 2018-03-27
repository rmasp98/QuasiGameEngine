#ifndef XXX_JSON_ARRAY_HPP
#define XXX_JSON_ARRAY_HPP

#include "utils/jsonFileManager/jsonUtils.hpp"
#include "utils/logging/logger.hpp"

#include "utils/jsonFileManager/rapidjson/document.h"
#include "utils/jsonFileManager/rapidjson/istreamwrapper.h"
namespace rj = rapidjson;


namespace xxx {
   class JsonArrayObject {
      friend class JsonFile;
   public:
      // Need to find a way to protect this constructor
      JsonArrayObject(rj::Value* arrayIn, Logger* loggerIn)
                    : _array(arrayIn), _size(arrayIn->Size()), _logger(loggerIn) {};
      ~JsonArrayObject() {};

      uint getSize() { return _size; };

      template<typename TYPE>
      TYPE getKey(uint element, const char* newKey) {
         if (element < _size) {
            rj::Value* newObject = extractKey(element, newKey);

            TYPE keyOut;
            json::getKey(newObject, keyOut, _logger);

            return keyOut;
         }

         LOG(LOG_ERROR, _logger) << "Json array does not have " << element
                                << " elements";
         throw "";
      }

      bool checkKey(uint element, const char* newKey) {
         return _array->operator[](element).HasMember(newKey);
      }


   private:
      rj::Value* _array;
      uint _size;
      Logger* _logger;

      rj::Value* extractKey(uint element, const char* newKey) {
         LOG(LOG_TRACE, _logger) << "Checking to see if '" << newKey << "' key exists";
         if (_array->operator[](element).HasMember(newKey))
            return &(_array->operator[](element)[newKey]);

         LOG(LOG_ERROR, _logger) << "Cannot find key " << newKey;
         throw "";
      }

   };
}


#endif // XXX_JSON_ARRAY_HPP
