/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_LOG_WORKER_H
#define QGE_LOG_WORKER_H

#include "utils/logging/log_data.h"
#include "utils/logging/log_source.h"

#include <unordered_map>
#include <utility>

namespace quasi_game_engine {

enum EngineComponent { TEST, GENERAL };

class LogWorker {
 public:
  void SendLog(EngineComponent component, LogData log) {
    auto source = sources_.find(component);
    assert(source != sources_.end());
    source->second.AddLogToQueue(log.GetLog());
  }

  void RegisterComponent(EngineComponent component, LogSource&& source) {
    sources_.emplace(component, std::move(source));
  }

  void ProcessLogs() {
    for (auto iter = sources_.begin(); iter != sources_.end(); ++iter) {
      while (iter->second.GetNumLogsInQueue()) {
        iter->second.ProcessNextLog();
      }
    }
  }

 private:
  std::unordered_map<EngineComponent, LogSource> sources_;
};

}  // namespace quasi_game_engine

#endif  // QGE_LOG_WORKER_H
