/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#include "utils/jsonFileManager/json_file_manager.h"
#include "utils/logging/logger.h"

namespace quasi_game_engine {

JsonFileManager::JsonFileManager(Logger* logger) {
  FileManager::SetLogger(logger);
}

JsonFile JsonFileManager::LoadFile(const char* file_name) {
  // This should be handled by filemanager
  if (file_name == nullptr) {
    LOG(LOG_ERROR, logger_) << "No input configuration file defined";
    //Set up throwing properly
    throw;
  }

  LOG(LOG_INFO, logger_) << "Loading input configuration from '" << file_name
                         << "'";
  JsonFile file(logger_);
  if (file.ParseFile(file_name))
    return file;
  else {
    LOG(LOG_ERROR, logger_) << "Unable to parse file: " << file_name
                            << ". Are you sure it exists?";
    throw;
  }
}

}  // namespace quasi_game_engine
