/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#include "utils/json_file.h"
#include "utils/logging/log_capture.h"

namespace quasi_game_engine {

nlohmann::json JsonFile::LoadFile(const char* file_name) {
  if (file_name == nullptr) {
    LOG(ERROR, UTILS) 
        << "No input configuration file defined";
    //TODO: Set up throwing properly
    throw "";
  }
  LOG(INFO, UTILS) 
      << "Loading input configuration from '" << file_name << "'";                    
  return ParseFile(file_name);
}


nlohmann::json JsonFile::ParseFile(const char* file_name) {
  LOG(INFO, UTILS) 
      << "Parsing new json schema from '" << file_name << "'";
  try {
    std::ifstream file_stream(file_name);
    nlohmann::json j = nlohmann::json::parse(file_stream);
    return j;
  } catch (nlohmann::json::parse_error execption) {
    LOG(ERROR, UTILS) << "Some error about json";
    //TODO: Throw an error associated with the engine
    throw;
  }
}

}  // namespace quasi_game_engine
