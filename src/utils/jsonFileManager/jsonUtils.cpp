
#include "utils/jsonFileManager/jsonUtils.hpp"
#include "utils/jsonFileManager/jsonArray.hpp"
#include "utils/logging/logger.hpp"

namespace xxx {
   namespace json {

      template<>
      void getKey<const char*>(rj::Value* keyIn, const char*& keyOut, Logger* logger) {
         if (keyIn->IsString())
            keyOut = keyIn->GetString();
         else {
            LOG(LOG_ERROR, logger) << "This key does not contain a string";
            throw "";
         }
      }

      template<>
      void getKey(rj::Value* keyIn, uint& keyOut, Logger* logger) {
         if (keyIn->IsUint())
            keyOut = keyIn->GetUint();
         else {
            LOG(LOG_ERROR, logger) << "This key does not contain an unsigned integer";
            throw "";
         }
      }

      template<>
      void getKey(rj::Value* keyIn, int& keyOut, Logger* logger) {
         if (keyIn->IsInt())
            keyOut = keyIn->GetInt();
         else {
            LOG(LOG_ERROR, logger) << "This key does not contain an integer";
            throw "";
         }
      }

      template<>
      void getKey(rj::Value* keyIn, float& keyOut, Logger* logger) {
         if (keyIn->IsFloat())
            keyOut = keyIn->GetFloat();
         else {
            LOG(LOG_ERROR, logger) << "This key does not contain a float";
            throw "";
         }
      }

      template<>
      void getKey(rj::Value* keyIn, bool& keyOut, Logger* logger) {
         if (keyIn->IsBool())
            keyOut = keyIn->GetBool();
         else {
            LOG(LOG_ERROR, logger) << "This key does not contain a boolean";
            throw "";
         }
      }

      template<>
      void getKey(rj::Value* keyIn, float*& keyOut, Logger* logger) {
         if (keyIn->IsArray()) {
            float* keyOut = new float[keyIn->Size()];
            for (uint iKey=0; iKey<keyIn->Size(); ++iKey)
               keyOut[iKey] = keyIn->operator[](iKey).GetFloat();
         } else {
            LOG(LOG_ERROR, logger) << "This key does not contain a float array";
            throw "";
         }
      }

      template<>
      void getKey(rj::Value* keyIn, JsonArrayObject*& keyOut, Logger* logger) {
         if (keyIn->IsArray())
            keyOut = new JsonArrayObject(keyIn, logger);
         else {
            LOG(LOG_ERROR, logger) << "This key does not contain an array object";
            throw "";
         }
      }


   }
}
