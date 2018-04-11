#ifndef QGE_LOGGER_HPP
#define QGE_LOGGER_HPP

#include <sstream>

namespace qge {
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


   std::string strcmb(std::initializer_list<const char*> list);

   struct Log {
      Log(LogLevel levelIn, std::string prefixIn, uint indexIn)
        : prefix(prefixIn), level(levelIn), fileIndex(indexIn) {};
      Log() {};

      std::string msg, prefix;
      LogLevel level;
      uint fileIndex;
      float time;
   };


   class Logger {
   public:
      Logger(const char* prefixIn, const char* fileIn, LogWorker* logWorkerIn);

      const char* getPrefix() { return _managerPrefix; };
      uint getFileIndex() { return _fileIndex; };
      void sendLog(Log log);

   private:
      const char *_managerPrefix;
      uint _fileIndex;
      LogWorker* _logWorker;
   };



   struct LogCapture {
      LogCapture(LogLevel levelIn, Logger* logger); // This should grab the time
      ~LogCapture(); //This will send the log to the queue

      std::ostringstream& stream() { return _stream; };

   private:
      std::ostringstream _stream;
      Log _log;
      Logger* _logger;
   };

#define LOG(level, logger) LogCapture(level, logger).stream()


} // ns qge


#endif // QGE_LOGGER_HPP
