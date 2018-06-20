#include "resource/resource_manager.h"


#include "resource/resource.h"
#include "utils/memory_manager.h"



namespace quasi_game_engine {

   ResourceManager::ResourceManager(LogWorker* log_worker)
      : logger_("[ Resource Manager ]", "logs/ResourceManager.log", log_worker),
        file_manager_(&logger_) {

      // Pass logger to all resources
      Resource::SetLogger(&logger_);

      //need to start by creating memory for resources

      //then initialise file manager for resources

      //then initialise the graphics manager if it hasn't already

      //initialise queue if needed

      //start thread for async loading
   }




   ResourceManager::~ResourceManager() {
      //run through map deleting all resources

      //delete memory

      //delete resource file manager
   }





   Resource* ResourceManager::GetResourceFromDB(int guid) {
      //get the rosource from the db...

      return NULL;
   }




   // (check if has been loaded, if not load)
   /*Model* ResourceManager::getModel(int guid, std::string fileLocation) {

      Model* modelPointer;

      //check to see if it has been loaded yet - if so return reference

      //if not we will load it given certain circumstances (maybe resources have already been loaded or in development mode)
      //we need to then load all resources and send some to the graphics card

      //if not we are going to throw an assertion

      return modelPointer;
   }*/





   Resource* ResourceManager::GetResource(int guid) {

      return GetResourceFromDB(guid);

   }

   //get resource from DB, if not there either throw an error or load the resource
   Resource* ResourceManager::GetResource(Asset asset) {

      //check to see if it is loaded, if so return pointer
      Resource* db_resource = GetResourceFromDB(asset.guid);
      if (db_resource != NULL)
         return db_resource;


      LOG(LOG_TRACE, &logger_) << "Resource has not been loaded yet! "
                  << "In release mode, this will cause the game to exit";

      return LoadResource(asset);
   }


   Resource* ResourceManager::GetResource(const char* file_location,
                                          const char* asset_name)
   {
      std::string name;
      if (asset_name == NULL)
         name = "";
      else
         name = asset_name;

      Asset asset(file_location, name);
      asset.guid = file_manager_.CalcGUID(asset.file_location, asset.asset_name);
      return GetResource(asset);

   }



   //Asychronous loading - will probably need a mutex for this
   /*Model* ResourceManager::asyncLoadModel(int guid, std::string fileLocation) {

      Model* modelPointer;

      //check model has not already been loaded

      //probablt need to mutex

      //then add file to an async load queue (probably need a priority)

      return modelPointer;
   }*/




   /*Resource* ResourceManager::asyncLoadResource(int guid, std::string fileLocation) {

      Resource* resourcePointer;

      //check resource has not already been loaded

      //probablt need to mutex

      //then add file to an async load queue (probably need a priority)

      return resourcePointer;
   }*/




   // I assume ths will need to be asynchronous
   //stream resource






   // This may need other information such as filePath
   // remove model from reference and delete all resources that haev parent count of 1
   void ResourceManager::DeleteModel(Model* model) {
      // will probably need to do some checks
      //then just call destructor for model
   }


   //Maybe delete all children with parent count of 1?
   void ResourceManager::DeleteResource(Resource* resource) {
      // will probably need to do some checks
      //then just call destructor for resource
   }


   /*Model* ResourceManager::loadModel(int guid, std::string fileLocation) {

      Model* modelPointer;

      //do stuff

      return modelPointer;

   }*/


   Resource* ResourceManager::LoadResource(Asset asset) {

      //This should just be an output of the reading of the file
      return file_manager_.LoadFile(asset);

   }

} // namespace quasi_game_engine
