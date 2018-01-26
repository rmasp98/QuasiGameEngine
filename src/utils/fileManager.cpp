#include "utils/fileManager.hpp"
#include "utils/logger.hpp"



namespace xxx {

   std::string FileManager::getExtension(std::string fileLoction) {
      logger->log("Getting file extension", LOG_TRACE);

      size_t i = fileLoction.rfind('.', fileLoction.length());
      if (i != std::string::npos) {
         return fileLoction.substr(i+1, fileLoction.length() - i);
      }

      //assert that file does not have extension
      logger->log(fileLoction + " does not have an extension. Cannot determine file type",
                  LOG_WARN);

      return NULL;
   }



   bool FileManager::compareFileType(std::string fileLoction,
                                     std::vector<std::string> extensions) {

      logger->log("Finding loader for this file type", LOG_TRACE);

      std::string ext = getExtension(fileLoction);
      for (uint iExt=0; iExt < extensions.size(); ++iExt) {
         if (!ext.compare(extensions[iExt]))
            return true;
      }

      return false;
   }

}
