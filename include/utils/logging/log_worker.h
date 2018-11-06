#ifndef QGE_LOG_WORKER_H
#define QGE_LOG_WORKER_H

#include <chrono>
#include <condition_variable>
#include <fstream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace quasi_game_engine {
class Log;
class Logger;

class LogWorker {
  friend Logger;

 public:
  LogWorker() : is_init_(false){};
  ~LogWorker();

  // If we have a static class ths will be used to initialise the class e.g
  // create thread
  bool Init();

 protected:
  void Push(Log log);
  int AddLogger(const char* file_name);

 private:
  std::queue<Log> log_queue_;
  std::mutex queue_mutex_, loop_mutex_, files_mutex_;
  std::thread* worker_thread_;
  std::condition_variable queue_wait_cv_;
  std::vector<std::ofstream> file_streams_;
  bool is_running_, is_init_;
  size_t max_size_;
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time_;

  bool Pop(Log& log);
  void CollectorLoop();
};

}  // namespace quasi_game_engine

#endif  // QGE_LOG_WORKER_H
