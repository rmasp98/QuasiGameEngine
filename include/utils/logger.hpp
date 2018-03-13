#ifndef XXX_LOG_MANAGER_HPP
#define XXX_LOG_MANAGER_HPP

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cassert>


#include <sstream>
#include <fstream>
#include <thread>
#include <queue>
#include <mutex>

namespace xxx {
   class RenderManager;

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
      Log(LogLevel levelIn, float timeIn) : level(levelIn), time(timeIn) {};
      Log() {};

      std::string msg, prefix;
      LogLevel level;
      uint fileIndex;
      float time;
   };



   class LogWorker;

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

   class LogWorker {
      friend Logger;
   public:
      LogWorker() :_isInit(false) {};
      ~LogWorker();

      // If we have a static class ths will be used to initialise the class e.g create thread
      bool init();

   protected:
      void push(Log log);
      uint addLogger(const char* fileName);


   private:
      std::queue<Log> logQueue;
      std::mutex queueMutex;
      std::thread* _workerThread;
      std::vector<std::ofstream> fileStreams;
      bool _isRunning, _isInit;
      uint maxSize;

      bool pop(Log& log);
      void collectorLoop();

   };

}


#endif // XXX_LOG_MANAGER_HPP
