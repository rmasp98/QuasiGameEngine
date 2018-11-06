#include "utils/file_manager.h"
#include "utils/logging/logger.h"

namespace quasi_game_engine {

std::string FileManager::GetExtension(std::string file_loction) {
  LOG(LOG_TRACE, logger_) << "Getting file extension";

  size_t i = file_loction.rfind('.', file_loction.length());
  if (i != std::string::npos) {
    return file_loction.substr(i + 1, file_loction.length() - i);
  }

  // assert that file does not have extension
  LOG(LOG_WARN, logger_)
      << file_loction
      << " does not have an extension. Cannot determine file type";

  return NULL;
}

bool FileManager::CompareFileType(const char* file_loction,
                                  QgeArray<const char*> extensions) {
  LOG(LOG_TRACE, logger_) << "Finding loader for this file type";

  std::string ext = GetExtension(file_loction);
  for (uint i_ext = 0; i_ext < extensions.Size(); ++i_ext) {
    if (!ext.compare(extensions[i_ext])) return true;
  }

  return false;
}

}  // namespace quasi_game_engine
