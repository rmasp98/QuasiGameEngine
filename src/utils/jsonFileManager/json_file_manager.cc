
#include "utils/jsonFileManager/json_file_manager.h"
#include "utils/logging/logger.h"

namespace quasi_game_engine {


   JsonFileManager::JsonFileManager(Logger* logger) : file_(logger), file_name_("") {
      FileManager::SetLogger(logger);
   }


   JsonFileManager::~JsonFileManager() {}


   JsonFile* JsonFileManager::LoadFile(const char* new_file_name) {
      //This should be handled by filemanager
      if (new_file_name != nullptr)
         file_name_ = new_file_name;
      else if (file_name_ == nullptr) {
         LOG(LOG_ERROR, logger_) << "No input configuration file defined";
         return nullptr;
      }

      LOG(LOG_INFO, logger_) << "Loading input configuration from '" << file_name_ << "'";

      if (file_.ParseFile(file_name_))
         return &file_;
      else
         return nullptr;
   }

} // namespace quasi_game_engine
