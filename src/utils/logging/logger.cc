#include "utils/logging/logger.h"
#include "utils/logging/log_worker.h"

#include <cassert>

// Will get rid of this later
#include <GLFW/glfw3.h>

namespace quasi_game_engine {

Logger::Logger(const char* prefix, const char* file, LogWorker* log_worker)
    : manager_prefix_(prefix),
      file_index_(log_worker->AddLogger(file)),
      log_worker_(log_worker) {}

// Logger& Logger::operator=(const Logger& new_logger) {
//   this->manager_prefix_ = new_logger.manager_prefix_;
//   this->file_index_ = new_logger.file_index_;
//   this->log_worker_ = new_logger.log_worker_;
//   return *this;
// }

void Logger::SendLog(Log log) { log_worker_->Push(log); }

LogCapture::LogCapture(LogLevel level, Logger* logger)
    : logger_(logger),
      log_(level, logger_->GetPrefix(), logger_->GetFileIndex()) {}

LogCapture::LogCapture(LogLevel level, const Logger* logger)
    : logger_(const_cast<Logger*>(logger)),
      log_(level, logger_->GetPrefix(), logger_->GetFileIndex()) {}

LogCapture::~LogCapture() {
  log_.msg = stream_.str();
  logger_->SendLog(log_);
}

}  // namespace quasi_game_engine
