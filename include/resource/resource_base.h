/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_RESOURCE_BASE_H
#define QGE_RESOURCE_BASE_H

#include "resource/resource.h"
#include "utils/logging/logger.h"

#include <string>

namespace quasi_game_engine {

//Struct to hold file metadata for resources
struct Asset {
  Asset(const char* file_location_in, const char* asset_name_in = NULL,
        unsigned int guid_in = 0)
      : file_location(file_location_in),
        name(asset_name_in),
        guid(guid_in) {}

  const char* file_location;
  const char* name;
  unsigned int guid;
};

//As renderer is not really used here, we will just forward declare it
class Renderer;

class ResourceBase : public Resource {
/*------------------------------------------------------------------------------
  Abstract class for all shared resource functionality including logger,
  parneting, file metadata and covering any functionality that does not apply to
  all resource types (e.g. rendering)
Note
- logger breaks in the case of multuiple resource managers
- Need to add functionality for adding, deleting and checking parents and children
------------------------------------------------------------------------------*/
 public:
  virtual ~ResourceBase() = default;

  //Getting rid of copy/move constructors/assignment operators (may need later)
  ResourceBase(const ResourceBase&) = delete;
  ResourceBase& operator=(const ResourceBase&) = delete;
  ResourceBase(ResourceBase&&) = delete;
  ResourceBase& operator=(ResourceBase&&) = delete;

  // Currently returns vao index (opengl) but need to find out directx equivilent
  virtual const int GetResource() const = 0;

  // Calls this function if child class does not render
  virtual void LoadToGraphics(Renderer* render_manager);

  //This will set the logger for all resources. Done in ResourceManager
  static void SetLogger(Logger* logger) { logger_ = logger; };

 protected:
  explicit ResourceBase(Asset asset);

  Asset asset_;
  //This needs to be a pointer to prevent requirement for default Logger constructor
  static Logger* logger_;
};

}  // namespace quasi_game_engine

#endif  // QGE_RESOURCE_BASE_H
