#include "resource/resourceManager.hpp"




namespace xxx {

   ResourceManager::ResourceManager(RenderManager* rendManIn) : renderMan(rendManIn) {
      fileMan = new ResourceFileManager();

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





   bool checkResourceDB(int guid) {
      //search algorithm for db

      return false;
   }




   Resource* getResourceFromDB(int guid) {
      //get the rosource from the db...
      Resource* tempResourcePointer = new Mesh();

      return tempResourcePointer;
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







   //get resource from DB, if not there either throw an error or load the resource
   Resource* ResourceManager::getResource(int guid, std::string fileLocation) {

      //check to see if it is loaded, if so return pointer
      if (checkResourceDB(guid)) {
         return getResourceFromDB(guid);
      }

// If we are in release mode we will throw an error here
#if XXX_RELEASE_MODE
      //assert that resource is not yet loaded, use asyncLoad first
#endif

      return loadResource(guid, fileLocation);
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


   Resource* ResourceManager::loadResource(int guid, std::string fileLocation) {

      //This should just be an output of the reading of the file
      Resource* resourcePointer = fileMan->loadFile(fileLocation);
      if (resourcePointer != NULL)
         resourcePointer->renderMan = renderMan;

      return resourcePointer;

   }

}
