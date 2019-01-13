/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_FILE_UTILS_H
#define QGE_FILE_UTILS_H

#include "utils/data_types/qge_array.h"

#include <fstream>
#include <string>

namespace quasi_game_engine {
namespace file_utils {

  // Gets string past final full stop
  const char* GetExtension(const char* file_loction);

  bool CompareFileType(const char* file_loction,
                       QgeArray<const char*> extensions);

}  // namespace file_utils
}  // namespace quasi_game_engine

#endif  // QGE_FILE_UTILS_H
