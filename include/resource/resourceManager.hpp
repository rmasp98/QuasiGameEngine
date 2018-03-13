#ifndef XXX_RESOURCE_MANAGER_HPP
#define XXX_RESOURCE_MANAGER_HPP

#include <string>

namespace xxx {
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
      ResourceManager(LogWorker* logWorkerIn);
      virtual ~ResourceManager();

      Model* getModel(int guid, std::string fileLocation); // (check if has been loaded, if not load)

      Resource* getResource(const char* fileLocation, const char* assetName = NULL);
      Resource* getResource(int guid);
      Resource* getResource(Asset asset);

      //Asychronous loading - will probably need a mutex for this
      Model* asyncLoadModel(int guid, std::string fileLocation);
      Resource* asyncLoadResource(int guid, std::string fileLocation);

      // I assume ths will need to be asynchronous
      //stream resource

      // create model (this will probably be an interactive function e.g. called from in game)

      // This may need other information such as filePath
      void deleteModel(Model* model); // remove model from reference and delete all resources that haev parent count of 1
      void deleteResource(Resource* resource); //Maybe delete all children with parent count of 1?

      //createModel for development purposes? To follow on from get resource to create a model from it

   protected:
      //resource reference table of resources type=map<int(GUID), pointer(to resource)>

      Resource* getResourceFromDB(int guid);

      // File manager (this will hopefully have versetile functions to load lots of different types of resource)
      //Need to find a way ro identify file types (custom loader for mine, assimp for models, SOIL for textures, etc.)
      ResourceFileManager* fileMan;
      MemoryManager* memMan;
      Logger* logger;

      Model* loadModel(int guid, std::string fileLocation);
      Resource* loadResource(Asset asset);

   };
}


#endif // XXX_RESOURCE_MANAGER_HPP
