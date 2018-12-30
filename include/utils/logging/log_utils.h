/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_LOG_UTILS_H
#define QGE_LOG_UTILS_H

#include <string>

namespace quasi_game_engine {

//If you need to add another logging source, add it before SIZE_LOGGER
// and iterator SIZE_LOGGER's value
enum EngineComponent {
  GENERAL = 0,
  INTERFACE = 1,
  PHYSICS = 2,
  RENDERER = 3,
  RESOURCE = 4,
  UI = 5,
  UTILS = 6,
  COMPONENT_SIZE = 7
};

enum LogLevel {
  FATAL = 0,
  ERROR = 1,
  WARN = 2,
  INFO = 3,
  DEBUG = 4,
  TRACE = 5
};

struct Log {
  Log(LogLevel level_in, std::string msg_in)
      : msg(msg_in), level(level_in) {};
  Log() {};

  std::string msg;
  LogLevel level;
  float time;
};

}  // namespace quasi_game_engine

#endif  // QGE_LOG_UTILS_H
