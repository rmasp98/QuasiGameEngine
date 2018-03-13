#ifndef XXX_FILE_MANAGER_HPP
#define XXX_FILE_MANAGER_HPP

#include <string>
#include <vector>
#include <fstream>



namespace xxx {
   class Logger;

   class FileManager {
   public:
      virtual ~FileManager() {};

      void setLogger(Logger* logIn) { logger = logIn; };

   protected:
      Logger* logger;

      FileManager() {};
      std::string getExtension(std::string fileLoction);
      bool compareFileType(std::string fileLoction, std::vector<std::string> extensions);
   };


}


#endif // XXX_FILE_MANAGER_HPP
