#include "utils/logger.hpp"

#include <chrono>
#include <cstdarg>



namespace xxx {



   std::string strcmb(std::initializer_list<const char*> list) {
      std::string outString;
      for( auto elem : list ) {
         outString += elem;
      }

      return outString;
   }




   Logger::Logger(const char* prefixIn, const char* fileIn, LogWorker* logWorkerIn) :
           _managerPrefix(prefixIn), _fileIndex(logWorkerIn->addLogger(fileIn)),
           _logWorker(logWorkerIn) {}


   void Logger::sendLog(Log log) {
      _logWorker->push(log);
   }



   LogCapture::LogCapture(LogLevel levelIn, Logger* loggerIn)
                       : _log(levelIn, 5 /*window->getTime*/),
                         _logger(loggerIn) {
      _log.prefix = _logger->getPrefix();
      _log.fileIndex = _logger->getFileIndex();
   }


   LogCapture::~LogCapture() {

      _log.msg = _stream.str();

      //printf("%s %s\n", _log.prefix.c_str(), _log.msg.c_str());

      _logger->sendLog(_log);

   }



   LogWorker::~LogWorker() {
      _isRunning = false;
      if (_workerThread->joinable()) {
         _workerThread->join();
      }
      delete _workerThread;
   }


   bool LogWorker::init() {
      if (!_isInit) {
         //initialise the class
         _isRunning = true;
         maxSize = 100;

         _workerThread = new std::thread(&LogWorker::collectorLoop, this);
         _isInit = true;
         return true;
      }

      // need to decide if we return false when already initialised
      return false;
   }


   void LogWorker::collectorLoop() {
      while (_isRunning || !logQueue.empty()) {
         Log log;
         if (pop(log)) {
            std::cout << log.prefix << " " << log.time << " " << log.msg << std::endl;

            //probably need to have a switch for writing to file
            if (log.fileIndex < fileStreams.size()) {
               fileStreams[log.fileIndex] << log.prefix << " " << log.time << " "
                                          << log.msg << std::endl;
            }
         }
      }
      printf("%s\n", "Closing logger");
   }


   void LogWorker::push(Log log) {
      if (logQueue.size() < maxSize) {
         std::lock_guard<std::mutex> guard(queueMutex);
         logQueue.push(log);
      }
   }


   uint LogWorker::addLogger(const char* fileName) {
      fileStreams.push_back(std::ofstream(fileName));
      return fileStreams.size() - 1;
   }


   bool LogWorker::pop(Log& log) {
      if (!logQueue.empty()) {
         log = logQueue.front();
         logQueue.pop();
         return true;
      }
      return false;
   }










}
