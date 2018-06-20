#ifndef QGE_INPUT_H
#define QGE_INPUT_H

#include "interface/action_list.h"

namespace quasi_game_engine {

class Input {
/*Notes-------------------------------------------------------------------------
- Notes go here
------------------------------------------------------------------------------*/
  friend class GlfwInputHelper;
 public:
  Input() = default;
  virtual ~Input() = default;

  //Getting rid of copy/move constructors/assignment operators (may need later)
  Input(const Input&) = delete;
  Input& operator=(const Input&) = delete;
  Input(Input&&) = delete;
  Input& operator=(Input&&) = delete;

  virtual void Update() = 0;
  virtual const float* GetMouseMovement() const = 0;
  virtual bool IsActionActive(const ActionEnum action) const = 0;
  virtual const ActionList* GetActionList() const = 0;

 protected:
  void operator delete (void *) {};
};

} // namespace quasi_game_engine

#endif // QGE_INPUT_H
