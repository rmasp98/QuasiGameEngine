
#include "utils/jsonFileManager/json_utils.h"
#include "utils/jsonFileManager/json_array.h"
#include "utils/logging/logger.h"

namespace quasi_game_engine {
   namespace json {

      template<>
      void GetKey<const char*>(rj::Value* key, const char*& key_out, Logger* logger) {
         if (key->IsString())
            key_out = key->GetString();
         else {
            LOG(LOG_ERROR, logger) << "This key does not contain a string";
            throw "String";
         }
      }

      template<>
      void GetKey(rj::Value* key, uint& key_out, Logger* logger) {
         if (key->IsUint())
            key_out = key->GetUint();
         else {
            LOG(LOG_ERROR, logger) << "This key does not contain an unsigned integer";
            throw "Unsigned Integer";
         }
      }

      template<>
      void GetKey(rj::Value* key, int& key_out, Logger* logger) {
         if (key->IsInt())
            key_out = key->GetInt();
         else {
            LOG(LOG_ERROR, logger) << "This key does not contain an integer";
            throw "Integer";
         }
      }

      template<>
      void GetKey(rj::Value* key, float& key_out, Logger* logger) {
         if (key->IsFloat())
            key_out = key->GetFloat();
         else {
            LOG(LOG_ERROR, logger) << "This key does not contain a float";
            throw "Float";
         }
      }

      template<>
      void GetKey(rj::Value* key, bool& key_out, Logger* logger) {
         if (key->IsBool())
            key_out = key->GetBool();
         else {
            LOG(LOG_ERROR, logger) << "This key does not contain a boolean";
            throw "Boolean";
         }
      }

      template<>
      void GetKey(rj::Value* key, float*& key_out, Logger* logger) {
         if (key->IsArray()) {
            float* key_out = new float[key->Size()];
            for (uint iKey=0; iKey<key->Size(); ++iKey)
               key_out[iKey] = (*key)[iKey].GetFloat();
         } else {
            LOG(LOG_ERROR, logger) << "This key does not contain a float array";
            throw "Floating Point Array";
         }
      }

      template<>
      void GetKey(rj::Value* key, JsonArrayObject*& key_out, Logger* logger) {
         if (key->IsArray())
            key_out = new JsonArrayObject(key, logger);
         else {
            LOG(LOG_ERROR, logger) << "This key does not contain an array object";
            throw "Json Array";
         }
      }


   } // ns json
} // namespace quasi_game_engine
