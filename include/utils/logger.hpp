#ifndef XXX_LOG_MANAGER_HPP
#define XXX_LOG_MANAGER_HPP

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cassert>


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


   class Logger {
   public:
      Logger();

      void log(std::string message, LogLevel outLevel);
      void visualLog(std::string message, LogLevel outLevel, std::vector<float> position);

   private:
      LogLevel logLevel;

   };


}


#endif // XXX_LOG_MANAGER_HPP
