
#include "utils/jsonFileManager/jsonFileManager.hpp"
#include "utils/logging/logger.hpp"

namespace qge {


   JsonFileManager::JsonFileManager(Logger* logIn) : fileName("") {
      FileManager::setLogger(logIn);
      file = new JsonFile(logger);
   }


   JsonFileManager::~JsonFileManager() {
      if (file != nullptr)
         delete file;

      if (fileName != nullptr)
         delete fileName;
   }


   JsonFile* JsonFileManager::loadFile(const char* newFileName) {
      //This should be handled by filemanager
      if (newFileName != NULL)
         fileName = newFileName;
      else if (fileName == NULL) {
         LOG(LOG_ERROR, logger) << "No input configuration file defined";
         return nullptr;
      }

      LOG(LOG_INFO, logger) << "Loading input configuration from '" << fileName << "'";

      if (file->parseFile(fileName))
         return file;
      else
         return nullptr;
   }

} // namespace qge
