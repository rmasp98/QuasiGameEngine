
#include "utils/logging/logWorker.hpp"
#include "utils/logging/logger.hpp"

#include <iostream>


namespace xxx {

   LogWorker::~LogWorker() {
      _isRunning = false;

      // Releases collectorLoop in order to deconstruct
      cv.notify_all();

      if (_workerThread->joinable()) {
         _workerThread->join();
      }
      delete _workerThread;

      for (uint iFile=0; iFile < _fileStreams.size(); ++iFile) {
         _fileStreams[iFile].close();
      }
   }


   bool LogWorker::init() {
      if (!_isInit) {
         //initialise the class
         _isRunning = true;
         _maxSize = 100;

         _workerThread = new std::thread(&LogWorker::collectorLoop, this);
         _isInit = true;
         return true;
      }

      // need to decide if we return false when already initialised
      return false;
   }


   void LogWorker::collectorLoop() {
      while (_isRunning || !_logQueue.empty()) {
         //put a block if queue empty (cv_something?)
         if (_logQueue.empty()) {
            std::unique_lock<std::mutex> lk(_loopMutex);
            cv.wait(lk);
         }

         Log log;
         if (pop(log)) {
            std::cout << log.prefix << " " << log.time << " " << log.msg << std::endl;

            //probably need to have a switch for writing to file
            if (log.fileIndex < _fileStreams.size()) {
               //This prevents other thread from altering the vector while we acccess it
               std::lock_guard<std::mutex> guard(_filesMutex);
               _fileStreams[0] << log.prefix << " " << log.time << " "
                               << log.msg << std::endl;
            }
         }
      }
      printf("%s\n", "Closing logger");
   }


   void LogWorker::push(Log log) {
      if (_logQueue.size() < _maxSize) {
         //This is currently not safe... If window not initialised it will crash
         log.time = getTime();
         std::lock_guard<std::mutex> guard(_queueMutex);
         _logQueue.push(log);

         //Notifies collectorLoop that something has been placed on queue
         cv.notify_all();
      }
   }


   uint LogWorker::addLogger(const char* fileName) {
      std::lock_guard<std::mutex> guard(_filesMutex);
      _fileStreams.push_back(std::ofstream(fileName));
      return _fileStreams.size() - 1;
   }


   bool LogWorker::pop(Log& log) {
      if (!_logQueue.empty()) {
         log = _logQueue.front();
         _logQueue.pop();
         return true;
      }
      return false;
   }


}
