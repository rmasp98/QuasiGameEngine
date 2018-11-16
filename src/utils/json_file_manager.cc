/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#include "utils/json_file_manager.h"
#include "utils/logging/logger.h"

namespace quasi_game_engine {

JsonFileManager::JsonFileManager(Logger* logger) : logger_(*logger) {}

nlohmann::json JsonFileManager::LoadFile(const char* file_name) {
  if (file_name == nullptr) {
    LOG(LOG_ERROR, &logger_) << "No input configuration file defined";
    //TODO: Set up throwing properly
    throw "";
  }
  LOG(LOG_INFO, &logger_) << "Loading input configuration from '" << file_name
                         << "'";                    
  return ParseFile(file_name);
}


nlohmann::json JsonFileManager::ParseFile(const char* config_file_name) {
  LOG(LOG_INFO, &logger_) << "Parsing new json schema from '" << config_file_name
                         << "'";
  try {
    std::ifstream file_stream(config_file_name);
    nlohmann::json j = nlohmann::json::parse(file_stream);
    return j;
  } catch (nlohmann::json::parse_error execption) {
    LOG(LOG_ERROR, &logger_) << "Some error about json";
    //TODO: Throw an error associated with the engine
    throw;
  }
}

}  // namespace quasi_game_engine
