#ifndef QGE_LOG_WORKER_HPP
#define QGE_LOG_WORKER_HPP

#include <vector>
#include <fstream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>


namespace qge {
   class Log;
   class Logger;

   class LogWorker {
      friend Logger;
   public:
      LogWorker() :_isInit(false) {};
      ~LogWorker();

      // If we have a static class ths will be used to initialise the class e.g create thread
      bool init();

      void setTimeFunction(double (*funcIn)()) { getTime = funcIn; };

   protected:
      void push(Log log);
      uint addLogger(const char* fileName);
      double (*getTime)();

   private:
      std::queue<Log> _logQueue;
      std::mutex _queueMutex, _loopMutex, _filesMutex;
      std::thread *_workerThread;
      std::condition_variable _queueWaitCv;
      std::vector<std::ofstream> _fileStreams;
      bool _isRunning, _isInit;
      uint _maxSize;

      bool pop(Log& log);
      void collectorLoop();

   };

} // ns qge




#endif // QGE_LOG_WORKER_HPP
