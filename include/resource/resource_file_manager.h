#ifndef QGE_RESOURCE_FILE_MANAGER_H
#define QGE_RESOURCE_FILE_MANAGER_H

#include "utils/file_manager.h"

#include <string>
#include <vector>



namespace quasi_game_engine {
   class Resource;
   class Logger;
   struct Asset;

   class ResourceFileManager : public FileManager {
   public:
      ResourceFileManager(Logger* logger);
      ~ResourceFileManager();

      Resource* LoadFile(Asset asset);
      uint CalcGUID(std::string file_location, std::string asset_name);

   protected:
      std::vector<std::string> freeimg_extensions_, assimp_extensions_;

      Resource* LoadImage(Asset asset);
      Resource* LoadObject(Asset asset);

   };
} // namespace quasi_game_engine


#endif // QGE_RESOURCE_FILE_MANAGER_H
