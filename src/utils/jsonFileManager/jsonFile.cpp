
#include "utils/jsonFileManager/jsonFile.hpp"


#include "utils/jsonFileManager/rapidjson/istreamwrapper.h"
#include "utils/jsonFileManager/rapidjson/error/en.h"

//This should not be needed and file should be open in filemanger
#include <fstream>


namespace qge {

   bool JsonFile::parseFile(const char *cfgFileName) {
      LOG(LOG_INFO, logger) << "Parsing json schema from '" << cfgFileName << "'";

      std::ifstream cfgFileIn(cfgFileName);
      rj::IStreamWrapper cfgIn(cfgFileIn);

      doc.ParseStream(cfgIn);
      if (doc.HasParseError()) {
         LOG(LOG_ERROR, logger) << "Failed to parse config file. "
                                << rj::GetParseError_En(doc.GetParseError());

         return false;
      }

      return true;
   }



   rj::Value* JsonFile::extractKey(const char* newKey) {
      LOG(LOG_TRACE, logger) << "Checking to see if '" << newKey << "' key exists";
      if (doc.HasMember(newKey))
         return &(doc[newKey]);

      LOG(LOG_ERROR, logger) << "Cannot find key '" << newKey << "'";
      return nullptr;
   }

}  // namespave qge
