#ifndef QGE_LOGGER_H
#define QGE_LOGGER_H

#include <sstream>

namespace quasi_game_engine {
   class RenderManager;
   class LogWorker;

   enum LogLevel {
      LOG_FATAL = 0,
      LOG_ERROR = 1,
      LOG_WARN  = 2,
      LOG_INFO  = 3,
      LOG_DEBUG = 4,
      LOG_TRACE = 5
   };

   struct Log {
      Log(LogLevel level_in, std::string prefix_in, int index_in)
        : prefix(prefix_in), level(level_in), file_index(index_in) {};
      Log() {};

      std::string msg, prefix;
      LogLevel level;
      int file_index;
      float time;
   };


   class Logger {
   public:
      Logger(const char* prefix, const char* file, LogWorker* log_worker);

      const char* GetPrefix() { return manager_prefix_; };
      int GetFileIndex() { return file_index_; };
      void SendLog(Log log);

   private:
      const char *manager_prefix_;
      int file_index_;
      LogWorker *log_worker_;
   };



   struct LogCapture {
      LogCapture(LogLevel level, Logger* logger);
      LogCapture(LogLevel level, const Logger* logger);
      ~LogCapture(); //This will send the log to the queue

      std::ostringstream& Stream() { return stream_; };

   private:
      Logger* logger_;
      std::ostringstream stream_;
      Log log_;
      
   };

//This might be a no no
#define LOG(level, logger) LogCapture(level, logger).Stream()

} // namespace quasi_game_engine


#endif // QGE_LOGGER_H
