/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#include "utils/file_utils.h"
#include "utils/logging/log_capture.h"

namespace quasi_game_engine {

const char* file_utils::GetExtension(const char* file_location) {
  std::string file_location_str = file_location;
  size_t i = file_location_str.rfind('.', file_location_str.length());
  if (i != std::string::npos) {
    return file_location_str.substr(i + 1, file_location_str.length() - i)
        .c_str();
  }

  throw;
}

bool file_utils::CompareFileType(const char* file_loction,
                                 QgeArray<const char*> extensions) {
  std::string ext = GetExtension(file_loction);
  for (int i_ext = 0; i_ext < extensions.Size(); ++i_ext) {
    if (!ext.compare(extensions[i_ext])) return true;
  }

  return false;
}

}  // namespace quasi_game_engine
