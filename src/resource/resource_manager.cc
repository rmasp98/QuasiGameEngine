/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#include "resource/resource_manager.h"

namespace quasi_game_engine {

ResourceManager::ResourceManager(LogWorker* log_worker)
    : logger_("Resource", "logs/ResourceManager.log", log_worker),
      file_manager_(&logger_) {
  // Pass logger to all resources
  ResourceBase::SetLogger(&logger_);
}


// get resource from DB, if not there either throw an error or load the resource
std::shared_ptr<Resource> ResourceManager::GetResource(Asset asset) {
  // check to see if it is loaded, if so return pointer
  std::shared_ptr<Resource> db_resource = GetResourceFromDB(asset.guid);
  if (db_resource != NULL) return db_resource;

  LOG(LOG_TRACE, &logger_)
      << "Resource has not been loaded yet! "
      << "In release mode, this will cause the game to exit";

  return LoadResource(asset);
}


// Maybe delete all children with parent count of 1?
void ResourceManager::DeleteResource(Asset asset) {
  // will probably need to do some checks
  // then just call destructor for resource
}


std::shared_ptr<Resource> ResourceManager::GetResourceFromDB(int guid) const {
  // get the rosource from the db...

  return NULL;
}


std::shared_ptr<Resource> ResourceManager::LoadResource(Asset asset) {
  //TODO: This will break when using own memory allocator (because it will call delete on destruction)
  std::shared_ptr<Resource> resource(file_manager_.LoadFile(asset));
  return resource;
}

}  // namespace quasi_game_engine
