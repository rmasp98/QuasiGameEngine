#include "utils/logging/logger.hpp"
#include "utils/logging/logWorker.hpp"

#include <chrono>
#include <cassert>

//Will get rid of this later
#include <GLFW/glfw3.h>



namespace xxx {

   //Need to try and remove this
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
                       : _log(levelIn, loggerIn->getPrefix(), loggerIn->getFileIndex()),
                         _logger(loggerIn) {}


   LogCapture::~LogCapture() {

      _log.msg = _stream.str();
      _logger->sendLog(_log);

   }

}
