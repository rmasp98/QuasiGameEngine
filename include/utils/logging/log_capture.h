/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_LOG_CAPTURE_H
#define QGE_LOG_CAPTURE_H

#include "utils/logging/log_utils.h"

#include <sstream>

namespace quasi_game_engine {

struct LogCapture {
  LogCapture(LogLevel level, EngineComponent component);
  ~LogCapture();  // This will send the log to the queue

  template<typename T>
  LogCapture &operator<<(const T& message) {
    stream_ << message;
    return *this;
  }

 private:
  std::ostringstream stream_;
  LogLevel level_;
  EngineComponent componenet_;
};

#define LOG(level, component) LogCapture(level, component)

}  // namespace quasi_game_engine

#endif  // QGE_LOG_CAPTURE_H
