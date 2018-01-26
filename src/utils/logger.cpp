#include "utils/logger.hpp"


namespace xxx {

   Logger::Logger() : logLevel(LOG_TRACE) {

      //create the log file here

   }


   //This will output to terminal/logfile
   void Logger::log(std::string message, LogLevel outputLevel) {

      if (outputLevel <= logLevel) {
         std::cout << message << std::endl;
      }

      if (outputLevel == LOG_FATAL) {
         exit(EXIT_FAILURE);
      }

   }


   //This will output to the screen rather than terminal/logfile
   void Logger::visualLog(std::string message,
                              LogLevel outLevel,
                              std::vector<float> position)
   {

      //Need to interact with the rendermanager somehow

   }


}
