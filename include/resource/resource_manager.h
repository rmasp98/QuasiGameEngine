#ifndef QGE_RESOURCE_MANAGER_H
#define QGE_RESOURCE_MANAGER_H


#include "resource/resource_file_manager.h"
#include "utils/logging/logger.h"

#include <string>

namespace quasi_game_engine {
   class MemoryManager;
   class Resource;
   class Model;
   struct Asset;
   class Mesh;
   class ResourceFileManager;
   class Logger;
   class LogWorker;


   class ResourceManager {
      /*Notes----------------------------------------------------------------
      - Do we want to implemennt singleton?
      - model here is just a reference to all resources used in a single model
      - In file will be a list of resource GUIDS along with the file path to resource
      - Could split this up into model and resource manager classes
      - need a resource class with sub mesh, material, texture, etc. classes
      - may need to implement a way to find out if a resource has been used recently (can be used to delete unused resource)
      ----------------------------------------------------------------------*/
   public:
      ResourceManager(LogWorker* log_worker);
      virtual ~ResourceManager();

      Model* GetModel(int guid, std::string file_location); // (check if has been loaded, if not load)

      Resource* GetResource(const char* file_location, const char* asset_name = NULL);
      Resource* GetResource(int guid);
      Resource* GetResource(Asset asset);

      //Asychronous loading - will probably need a mutex for this
      Model* AsyncLoadModel(int guid, std::string file_location);
      Resource* AsyncLoadResource(int guid, std::string file_location);

      // I assume ths will need to be asynchronous
      //stream resource

      // create model (this will probably be an interactive function e.g. called from in game)

      // This may need other information such as filePath
      void DeleteModel(Model* model); // remove model from reference and delete all resources that haev parent count of 1
      void DeleteResource(Resource* resource); //Maybe delete all children with parent count of 1?

      //createModel for development purposes? To follow on from get resource to create a model from it

   protected:
      //resource reference table of resources type=map<int(GUID), pointer(to resource)>

      Resource* GetResourceFromDB(int guid);

      // File manager (this will hopefully have versetile functions to load lots of different types of resource)
      //Need to find a way ro identify file types (custom loader for mine, assimp for models, SOIL for textures, etc.)
      MemoryManager* memory_manager_;
      Logger logger_;
      ResourceFileManager file_manager_;

      Model* LoadModel(int guid, std::string file_location);
      Resource* LoadResource(Asset asset);

   };
} // namespace quasi_game_engine


#endif // QGE_RESOURCE_MANAGER_H
