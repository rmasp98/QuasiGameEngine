/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_LOG_WORKER_H
#define QGE_LOG_WORKER_H

#include "utils/qge_queue.h"
#include "utils/logging/log_utils.h"

#include <map>
#include <chrono>
#include <condition_variable>
#include <fstream>
#include <mutex>
#include <queue>
#include <thread>

namespace quasi_game_engine {

struct something {
  QgeQueue<Log> queue;
  const char* prefix;
  std::ofstream file_stream;
};

class LogWorker {
 public:
  LogWorker();
  ~LogWorker();

  //Getting rid of copy/move constructors/assignment operators (may need later)
  LogWorker(const LogWorker&) = delete;
  LogWorker& operator=(const LogWorker&) = delete;
  LogWorker(LogWorker&&) = delete;
  LogWorker& operator=(LogWorker&&) = delete;

  void SendLog(Log log, EngineComponent component);

 private:
  void CollectorLoop();

  something logger_list_[COMPONENT_SIZE];
  std::thread* worker_thread_;
  std::condition_variable queue_wait_cv_;
  std::mutex loop_mutex_;
  int largest_queue_size_, largest_queue_;
  //std::vector<std::ofstream> file_streams_;
  bool is_running_;
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time_;
};



}  // namespace quasi_game_engine

#endif  // QGE_LOG_WORKER_H
