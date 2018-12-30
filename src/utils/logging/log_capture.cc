/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#include "utils/logging/log_capture.h"
#include "utils/logging/log_worker.h"

// Will get rid of this later
#include <GLFW/glfw3.h>

namespace quasi_game_engine {

//construct log_worker_
static LogWorker log_worker_;

LogCapture::LogCapture(LogLevel level, EngineComponent component)
    : level_(level), componenet_(component) {}

LogCapture::~LogCapture() {
  log_worker_.SendLog(Log(level_, stream_.str()), componenet_);
}

}  // namespace quasi_game_engine
