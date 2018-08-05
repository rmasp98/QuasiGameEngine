/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_INPUT_H
#define QGE_INPUT_H

#include "interface/action_list.h"

namespace quasi_game_engine {

class Input {
/*------------------------------------------------------------------------------
  Interface for input classes of different window managers. Responsible for
  updating and storing ActionList and mouse input (potentially gamepad and
  others later). Can only be created and deleted by an XXXInterface class
Notes
- Do I need to GetActionList?
------------------------------------------------------------------------------*/
  friend class GlfwInputHelper;
 public:
  virtual ~Input() = default;

  //Getting rid of copy/move constructors/assignment operators (may need later)
  Input(const Input&) = delete;
  Input& operator=(const Input&) = delete;
  Input(Input&&) = delete;
  Input& operator=(Input&&) = delete;

  virtual void Update() = 0; // called every frame. Updates user input
  virtual const double* GetMouseMovement() const = 0; //diff from position last frame
  virtual const double* GetMousePosition() const = 0;
  virtual bool IsActionActive(ActionEnum action) const = 0;
  virtual const ActionList* GetActionList() const = 0;

 protected:
  Input() = default; // Doubly ensure no one can directly create this object

  // Interface manages lifetime of object but returns pointer to object
  // This prevents user from being able to accidentally delete object
  void operator delete (void *) {};
};

} // namespace quasi_game_engine

#endif // QGE_INPUT_H
