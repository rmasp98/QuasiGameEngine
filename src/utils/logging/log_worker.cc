/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#include "utils/logging/log_worker.h"
#include "utils/logging/log_capture.h"

namespace quasi_game_engine {

typedef std::chrono::high_resolution_clock timer;

LogWorker::LogWorker()
    : worker_thread_(new std::thread(&LogWorker::CollectorLoop, this)),
      largest_queue_size_(0),
      largest_queue_(0),
      is_running_(true),
      start_time_(timer::now()) {
  // TODO: Need to read this from a file?
  logger_list_[GENERAL].prefix = "[  General  ]";
  logger_list_[GENERAL].file_stream.open("logs/General.log");

  logger_list_[INTERFACE].prefix = "[ Interface ]";
  logger_list_[INTERFACE].file_stream.open("logs/Interface.log");

  logger_list_[PHYSICS].prefix = "[  Physics  ]";
  logger_list_[PHYSICS].file_stream.open("logs/Physics.log");

  logger_list_[RENDERER].prefix = "[  Renderer ]";
  logger_list_[RENDERER].file_stream.open("logs/Renderer.log");

  logger_list_[RESOURCE].prefix = "[  Resource ]";
  logger_list_[RESOURCE].file_stream.open("logs/Resource.log");

  logger_list_[UI].prefix = "[     UI     ]";
  logger_list_[UI].file_stream.open("logs/UI.log");

  logger_list_[UTILS].prefix = "[   Utils   ]";
  logger_list_[UTILS].file_stream.open("logs/Utils.log");
}

LogWorker::~LogWorker() {
  is_running_ = false;

  // Releases collectorLoop in order to deconstruct
  queue_wait_cv_.notify_all();

  if (worker_thread_->joinable()) {
    worker_thread_->join();
  }
  delete worker_thread_;
}

void LogWorker::SendLog(Log log, EngineComponent component) {
  QgeQueue<Log>* queue = &logger_list_[component].queue;

  std::chrono::duration<double> time_diff = timer::now() - start_time_;
  log.time = time_diff.count();

  try {
    queue->Push(log);
  } catch (...) {
    // Do something if cannot push log (probably create another collector loop
    // thread)
  }

  // Check to see if this makes it the biggest queue
  if (queue->Size() > largest_queue_size_) {
    largest_queue_ = component;
    largest_queue_size_ = queue->Size();
  }

  // Notifies collectorLoop that something has been placed on queue
  queue_wait_cv_.notify_one();
}

void LogWorker::CollectorLoop() {
  while (is_running_ || largest_queue_size_ > 0) {
    // put a block if queue empty
    if (largest_queue_size_ == 0) {
      // Check if there are any logs in other queues
      for (int i_logs = 0; i_logs < COMPONENT_SIZE; ++i_logs) {
        if (logger_list_[i_logs].queue.Size() > largest_queue_size_) {
          largest_queue_size_ = logger_list_[i_logs].queue.Size();
          largest_queue_ = i_logs;
        }
      }

      if (largest_queue_size_ == 0) {  // If still no logs in current queue wait
        std::unique_lock<std::mutex> lock(loop_mutex_);
        queue_wait_cv_.wait(lock);
      }
    }

    QgeQueue<Log>* queue = &logger_list_[largest_queue_].queue;
    if (!queue->Empty()) {
      Log log = queue->Pop();
      printf("%s %f %s\n", logger_list_[largest_queue_].prefix, log.time,
             log.msg.c_str());

      logger_list_[largest_queue_].file_stream
          << logger_list_[largest_queue_].prefix << " " << log.time << " "
          << log.msg << std::endl;

      largest_queue_size_ = queue->Size();
    }

    // Randomly check another queue to see if any are larger (perfect randomness
    // not really required)
    int random_queue = rand() % COMPONENT_SIZE;
    if (logger_list_[random_queue].queue.Size() > largest_queue_size_) {
      largest_queue_size_ = logger_list_[random_queue].queue.Size();
      largest_queue_ = random_queue;
    }
  }
  printf("Closing logger\n");
}

}  // namespace quasi_game_engine
