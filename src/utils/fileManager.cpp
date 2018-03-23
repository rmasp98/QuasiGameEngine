#include "utils/fileManager.hpp"
#include "utils/logging/logger.hpp"



namespace xxx {


   std::string FileManager::getExtension(std::string fileLoction) {
      LOG(LOG_TRACE, logger) << "Getting file extension";

      size_t i = fileLoction.rfind('.', fileLoction.length());
      if (i != std::string::npos) {
         return fileLoction.substr(i+1, fileLoction.length() - i);
      }

      //assert that file does not have extension
      LOG(LOG_WARN, logger) << fileLoction
                            << " does not have an extension. Cannot determine file type";

      return NULL;
   }



   bool FileManager::compareFileType(std::string fileLoction,
                                     std::vector<std::string> extensions) {

      LOG(LOG_TRACE, logger) << "Finding loader for this file type";

      std::string ext = getExtension(fileLoction);
      for (uint iExt=0; iExt < extensions.size(); ++iExt) {
         if (!ext.compare(extensions[iExt]))
            return true;
      }

      return false;
   }

}
