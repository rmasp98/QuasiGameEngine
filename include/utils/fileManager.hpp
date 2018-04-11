#ifndef QGE_FILE_MANAGER_HPP
#define QGE_FILE_MANAGER_HPP

#include <string>
#include <vector>
#include <fstream>



namespace qge {
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


} // ns qge


#endif // QGE_FILE_MANAGER_HPP
