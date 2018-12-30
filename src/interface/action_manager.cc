/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#include "interface/action_manager.h"

#include "utils/logging/log_capture.h"

#include <cmath>


namespace quasi_game_engine {

static long active_actions_ = 0;
static Action action_list_[ACTION_SIZE];

ActionManager::ActionManager(const char* config_file) {
  LoadActions(config_file);
}


void ActionManager::Update(std::unordered_set<int> pressed_keys) {
  int old_action_state = action_state_;
  action_state_ = 0;
  for (const auto &elem : pressed_keys) {
    Action* action = button_list_[elem];
    if (action != nullptr && !(action->bit_position & action_state_)) {
      action_state_ += action->bit_position;
    }
  }

  // button state changed if buttonState xor (not the same as) oldButtonState
  int state_change = action_state_ ^ old_action_state;
  // active buttons if the button is a hold button or has changed and is pressed down
  active_actions_ = (hold_actions_ | state_change) & action_state_;
}


void ActionManager::LoadActions(const char* config_file) {
  nlohmann::json json_config = json_file_.LoadFile(config_file);
  
  auto input_array = json_config["inputType"];
  for (auto& input : input_array) {
    std::string input_type_string = input["type"];
    if (!input_type_string.compare("keyboard")) {
      //ButtonTypeEnum input_type = GetButtonEnumFromString(input_type_string.c_str());
      nlohmann::json action_array = input["actions"];
      for (auto& action : action_array) {
        std::string name = action["name"];
        int button_value = action["val"];
        int action_value = action["action"];
        bool is_hold = action["hold"];

        //Make sure it is a valid ActionEnum, otherwise throw an exception
        if (action_value >= 0 && action_value < ACTION_SIZE) {
          action_list_[action_value] = Action(name.c_str(), is_hold);
          button_list_[button_value] = &action_list_[action_value];
        } else {
          throw;
        }
      }
    }
  }
  
  SetBitOrder();
}


bool ActionManager::IsActionActive(ActionEnum action) {
  return active_actions_ & action_list_[action].bit_position;
}


void ActionManager::SetBitOrder() {
  //set location of button in bit state
  hold_actions_ = 0;
  for (int i_action=0; i_action<ACTION_SIZE; ++i_action) { 
    action_list_[i_action].bit_position = pow(2, i_action);
    //If action should remain active when held down
    if (action_list_[i_action].is_hold)
      hold_actions_ += action_list_[i_action].bit_position;

  }
}


} // namespace quasi_game_engine
