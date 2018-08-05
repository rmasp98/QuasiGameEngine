#ifndef QGE_ACTION_H
#define QGE_ACTION_H

#include "utils/jsonFileManager/json_file_manager.h"
#include "utils/logging/logger.h"

#include <string>
#include <map>


namespace quasi_game_engine {

// Currently used for defining origin of button and button mapping when loading
enum ButtonTypeEnum {
  BUTTON_KEYBOARD, BUTTON_MOUSE, BUTTON_GAMEPAD,
  BUTTON_NULL
};

struct Button {
  Button(int value_in, ButtonTypeEnum type_in)
    : button_value(value_in), button_type(type_in) {};
  Button() : button_type(BUTTON_NULL) {};

  int button_value;
  ButtonTypeEnum button_type;
};

// All used buttons must have an associated action defined in this struct
// Each action is then mapped to a bit-chain - its location is defined by bit_location
class Action {
 public:
  Action(const char* name, bool is_hold, Button button)
    : name_(name), is_hold_(is_hold), num_buttons(0) { buttons_[num_buttons++] = button; };
  Action() {}; //TODO: need to work out why we need this?

  //This class will be regularly passed between function so require these semantics. TODO: may need to set explicit functions
   Action(const Action&) = default;
   Action& operator=(const Action&) = default;
   Action(Action&&) = default;
   Action& operator=(Action&&) = default;

  const char* GetName() const { return name_; };
  bool IsHold() const { return is_hold_; };
  const Button GetButtons(int element) const {
    if (element >= 0 && element < num_buttons) return buttons_[element];
    return Button();
  };
  int GetNumButtons() const { return num_buttons; };

 private:
  const char* name_;
  bool is_hold_;
  int num_buttons;
  Button buttons_[5];
};

} // namespace quasi_game_engine

#endif // QGE_ACTION_H
