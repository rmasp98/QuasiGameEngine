
#include "utils/logging/log_worker.h"
#include "utils/logging/logger.h"

#include <iostream>


namespace quasi_game_engine {

   typedef std::chrono::high_resolution_clock timer;

   LogWorker::~LogWorker() {
      is_running_ = false;

      // Releases collectorLoop in order to deconstruct
      queue_wait_cv_.notify_all();

      if (worker_thread_->joinable()) {
         worker_thread_->join();
      }
      delete worker_thread_;

      for (uint i_file=0; i_file < file_streams_.size(); ++i_file) {
         file_streams_[i_file].close();
      }
   }


   bool LogWorker::Init() {
      //Currently prevents multiple threads from being created could theoretically have mulitple...
      if (!is_init_) {
         //initialise the class
         is_running_ = true;
         max_size_ = 100;
         start_time_ = timer::now();

         worker_thread_ = new std::thread(&LogWorker::CollectorLoop, this);
         is_init_ = true;
         return true;
      }

      // need to decide if we return false when already initialised
      return false;
   }


   void LogWorker::CollectorLoop() {
      while (is_running_ || !log_queue_.empty()) {
         //put a block if queue empty
         if (log_queue_.empty()) {
            std::unique_lock<std::mutex> lock(loop_mutex_);
            queue_wait_cv_.wait(lock);
         }

         Log log;
         if (Pop(log)) {
            std::cout << log.prefix << " " << log.time << " " << log.msg << std::endl;

            //probably need to have a switch for writing to file
            if (log.file_index >= 0 && static_cast<uint>(log.file_index) < file_streams_.size()) {
               //This prevents other thread from altering the vector while we acccess it
               std::lock_guard<std::mutex> guard(files_mutex_);
               file_streams_[0] << log.prefix << " " << log.time << " "
                                << log.msg << std::endl;
            }
         }
      }
      printf("%s\n", "Closing logger");
   }


   void LogWorker::Push(Log log) {
      if (log_queue_.size() < max_size_) {
         std::chrono::duration<double> time_diff = timer::now() - start_time_;
         log.time = time_diff.count();

         // Locks queue so that log can be added
         std::lock_guard<std::mutex> guard(queue_mutex_);
         log_queue_.push(log);

         //Notifies collectorLoop that something has been placed on queue
         queue_wait_cv_.notify_one();
      }
   }


   int LogWorker::AddLogger(const char* file_name) {
      std::lock_guard<std::mutex> guard(files_mutex_);
      file_streams_.push_back(std::ofstream(file_name));
      return file_streams_.size() - 1;
   }


   bool LogWorker::Pop(Log& log) {
      if (!log_queue_.empty()) {
         log = log_queue_.front();

         //Need to check if queue is already thread safe?
         std::lock_guard<std::mutex> guard(queue_mutex_);
         log_queue_.pop();
         return true;
      }
      return false;
   }


} // namespace quasi_game_engine
