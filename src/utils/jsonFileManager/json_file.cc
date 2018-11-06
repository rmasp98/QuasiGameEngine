/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#include "utils/jsonFileManager/json_file.h"

#include "utils/jsonFileManager/rapidjson/error/en.h"
#include "utils/jsonFileManager/rapidjson/istreamwrapper.h"

// This should not be needed and file should be open in filemanger
#include <fstream>

namespace quasi_game_engine {

bool JsonFile::ParseFile(const char* config_file_name) {
  LOG(LOG_INFO, logger_) << "Parsing json schema from '" << config_file_name
                         << "'";

  std::ifstream file_stream(config_file_name);
  rj::IStreamWrapper rj_stream(file_stream);

  doc_->ParseStream(rj_stream);
  if (doc_->HasParseError()) {
    LOG(LOG_ERROR, logger_) << "Failed to parse config file. "
                            << rj::GetParseError_En(doc_->GetParseError());

    return false;
  }

  return true;
}

rj::Value* JsonFile::ExtractKey(const char* new_key) {
  LOG(LOG_TRACE, logger_) << "Checking to see if '" << new_key
                          << "' key exists";
  if (doc_->HasMember(new_key)) return &((*doc_)[new_key]);

  LOG(LOG_ERROR, logger_) << "Cannot find key '" << new_key << "'";
  return nullptr;
}

}  // namespace quasi_game_engine
