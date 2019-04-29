/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_RESOURCE_MANAGER_H
#define QGE_RESOURCE_MANAGER_H

#include "resource/resource.h"
#include "resource/resource_base.h"
#include "resource/resource_file_manager.h"
#include "utils/logging/log_worker.h"

#include <memory>

namespace quasi_game_engine {

class ResourceManager {
  /*------------------------------------------------------------------------------
    Stuff
  Notes
  - model here is just a reference to all resources used in a single model
  - may need to implement a way to find out if a resource has been used recently
    (can be used to delete unused resource)
  - Add streaming of resources such as video and sound
  - Asynchronous file load (should be main type) (needs own thread)
  - resource reference table of resources type=map<int(GUID), pointer(to
  resource)>
  - Memory management!
  - Resource storage should be a  seperate class
  ------------------------------------------------------------------------------*/
 public:
  ResourceManager() = default;
  ~ResourceManager() = default;

  // Getting rid of copy/move constructors/assignment operators (may need later)
  ResourceManager(const ResourceManager&) = delete;
  ResourceManager& operator=(const ResourceManager&) = delete;
  ResourceManager(ResourceManager&&) = delete;
  ResourceManager& operator=(ResourceManager&&) = delete;

  // This will try to get resource from database, but will load it if it does
  // not exist
  std::shared_ptr<Resource> GetResource(Asset asset);
  void DeleteResource(Asset asset);

 protected:
  // This function will go to the database class
  std::shared_ptr<Resource> GetResourceFromDB(int guid) const;
  std::shared_ptr<Resource> LoadResource(Asset asset);

  ResourceFileManager file_manager_;
};

}  // namespace quasi_game_engine

#endif  // QGE_RESOURCE_MANAGER_H
