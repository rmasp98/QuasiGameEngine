/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/
#ifndef QGE_ACTION_H
#define QGE_ACTION_H

#include "utils/json_file_manager.h"
#include "utils/logging/logger.h"

#include <string>
#include <map>


namespace quasi_game_engine {

// Currently used for defining origin of button and button mapping when loading
enum ButtonTypeEnum {
  BUTTON_KEYBOARD, BUTTON_MOUSE, BUTTON_GAMEPAD,
  BUTTON_NULL
};

// Stores all information of a physical button for example a key or gamepad button
struct Button {
  Button(int value_in, ButtonTypeEnum type_in)
    : button_value(value_in), button_type(type_in) {};
  Button() : button_type(BUTTON_NULL) {};

  int button_value;
  ButtonTypeEnum button_type;
};


class Action {
/*------------------------------------------------------------------------------
Responsible storing the information for a single action. This will be a
component to the  ActionList class. An action can have multiple associated buttons
Notes:
- Figure out why we need a default constructor
------------------------------------------------------------------------------*/
 public:
  Action(const char* name, bool is_hold, Button button)
    : name_(name), is_hold_(is_hold), num_buttons(0) { buttons_[num_buttons++] = button; };
  Action() {}; //TODO: need to work out why we need this?

  //This class will be regularly passed between function so require these semantics.
  //TODO: may need to set explicit functions
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
