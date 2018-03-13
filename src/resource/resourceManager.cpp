#include "resource/resourceManager.hpp"


#include "resource/resourceFileManager.hpp"
#include "resource/resource.hpp"
#include "utils/memoryManager.hpp"
#include "utils/logger.hpp"



namespace xxx {

   ResourceManager::ResourceManager(LogWorker* logWorkerIn) {

      // Create logger and pass it to all resources
      logger = new Logger("[Resource Manager]", "logs/ResourceManager.log", logWorkerIn);
      Resource::logger = logger;

      fileMan = new ResourceFileManager(logger);


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

      delete logger;
   }





   Resource* ResourceManager::getResourceFromDB(int guid) {
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





   Resource* ResourceManager::getResource(int guid) {

      return getResourceFromDB(guid);

   }

   //get resource from DB, if not there either throw an error or load the resource
   Resource* ResourceManager::getResource(Asset asset) {

      //check to see if it is loaded, if so return pointer
      Resource* dbResource = getResourceFromDB(asset.guid);
      if (dbResource != NULL)
         return dbResource;


      LOG(LOG_TRACE, logger) << "Resource has not been loaded yet! "
                  << "In release mode, this will cause the game to exit";

      return loadResource(asset);
   }


   Resource* ResourceManager::getResource(const char* fileLocation,
                                          const char* assetName)
   {
      std::string name;
      if (assetName == NULL)
         name = "";
      else
         name = assetName;

      Asset asset(fileLocation, name);
      asset.guid = fileMan->calcGUID(asset.fileLocation, asset.assetName);
      return getResource(asset);

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
   void ResourceManager::deleteModel(Model* model) {
      // will probably need to do some checks
      //then just call destructor for model
   }


   //Maybe delete all children with parent count of 1?
   void ResourceManager::deleteResource(Resource* resource) {
      // will probably need to do some checks
      //then just call destructor for resource
   }


   /*Model* ResourceManager::loadModel(int guid, std::string fileLocation) {

      Model* modelPointer;

      //do stuff

      return modelPointer;

   }*/


   Resource* ResourceManager::loadResource(Asset asset) {

      //This should just be an output of the reading of the file
      Resource* resourcePointer = fileMan->loadFile(asset);

      return resourcePointer;

   }

}
