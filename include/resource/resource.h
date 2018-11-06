/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_RESOURCE_H
#define QGE_RESOURCE_H

namespace quasi_game_engine {

//As renderer is not really used here, we will just forward declare it
class Renderer;

class Resource {
/*------------------------------------------------------------------------------
  Interface class for all resources such as meshes, textures, skeletons, etc.
Note
- Wanted to load files in constructors but we don't know the size before file is loaded
- Need to make skeleton, animation, material, font, etc. subclasses
- think about model class
- Need to think about unloading from memory?
------------------------------------------------------------------------------*/
 public:
  virtual ~Resource() = default;

  //Getting rid of copy/move constructors/assignment operators (may need later)
  Resource(const Resource&) = delete;
  Resource& operator=(const Resource&) = delete;
  Resource(Resource&&) = delete;
  Resource& operator=(Resource&&) = delete;

  // Currently returns openGL VAO index (need to work out DIrectX equivilent)
  virtual const int GetResource() const = 0;
  // Fully load a resource into graphics memory
  virtual void LoadToGraphics(Renderer* render_manager) = 0;

 protected:
  Resource() = default;
};

}  // namespace quasi_game_engine

#endif  // QGE_RESOURCE_H
