/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#include "interface/input.h"

#include "utils/logging/logger.h"

#include <cmath>


namespace quasi_game_engine {

ActionList::ActionList(Logger logger) : set_input_(BUTTON_KEYBOARD),
                                        logger_(logger),
                                        json_file_manager_(&logger) {
  //load from a config file
}


void ActionList::Update(int new_action_state) {
  int old_action_state = action_state_;

  // button state changed if buttonState xor (not the same as) oldButtonState
  int state_change = new_action_state ^ old_action_state;
  // active buttons if the button is a hold button or has changed and is pressed down
  active_actions_ = (hold_actions_ | state_change) & new_action_state;
  action_state_ = new_action_state;
}

//TODO: this should probably take input of vector of buttons
void ActionList::SetAction(ActionEnum action, int button_value, bool is_hold,
                           const char* name) {
  //TODO: Need to update this to check what the key type is
  actions_[action] = Action(name, is_hold,
                            Button(button_value, BUTTON_KEYBOARD));

  //If this is a new button assignment, it ensures order is maintained
  SetBitOrder();

  LOG(LOG_TRACE, &logger_) << "Setting action: Name = " << name
                           << "; Button Value = " << button_value
                           << "; Action Value = " << action;
}


void ActionList::SetAction(int action, int button_value, bool is_hold,
                           const char* name) {
  //This heavily relies on ActionEnums integrity
  ActionEnum action_convert;
  if (action <= ACTION_MAX) {
    action_convert = (ActionEnum)action;
    SetAction(action_convert, button_value, is_hold, name);
  } else {
    LOG(LOG_ERROR, &logger_) << "Action value " << action << " does not exist."
                             << " The largest value is " << ACTION_MAX;
  }
}


const Action ActionList::GetAction(ActionEnum action) const {
  if (actions_.find(action) != actions_.end()) { // if the action already exists
    return actions_.at(action);
  } else {
    LOG(LOG_WARN, &logger_) << "Action " << action
                            << " has not been set yet";
    return Action();
  }
}


const Action ActionList::GetAction(const char* name) const {
  for (auto const &action : actions_) {
    // Need to create a better string comparison
    if (action.second.GetName() == name)
      return action.second;
  }

  LOG(LOG_ERROR, &logger_) << name << " action does not exist";
  return Action();
}


void ActionList::LoadActionMapping(const char* config_file_name) {
  nlohmann::json config_file_new = json_file_manager_.LoadFile(config_file_name);
  ParseActions(config_file_new);
}


// void ActionList::ReloadActionMapping() {
//   JsonFile config_file = json_file_manager_.LoadFile();
//   ParseActions(&config_file);
// }


void ActionList::ParseActions(nlohmann::json config_file) {
  auto input_array = config_file["inputType"];
  for (auto& input : input_array) {
    std::string input_type_string = input["type"];
    ButtonTypeEnum input_type = GetButtonEnumFromString(input_type_string.c_str());
    nlohmann::json action_array = input["actions"];
    for (auto& action : action_array) {
      std::string name = action["name"];
      int button_value = action["val"];
      int action_value = action["action"];
      bool is_hold = action["hold"];

      SetAction(action_value, button_value, is_hold, name.c_str());
    }
  }
}


void ActionList::SetBitOrder() {
  //set location of button in bit state
  hold_actions_ = 0;
  for (auto const &action : actions_) {
    //If action should remain active when held down
    if (actions_[action.first].IsHold())
      hold_actions_ += pow(2, (int)action.first);

  }
}


//TODO: This should probably be a static function with active_buttons_ also as input
bool ActionList::IsActionActive(const ActionEnum action) const {
  return active_actions_ & (int)pow(2, (int)action);
}


ButtonTypeEnum ActionList::GetButtonEnumFromString(const char* input_type_str) const {
  LOG(LOG_INFO, &logger_) << "Loading button mapping for input type '"
                          << input_type_str << "'";
  if (!strcmp(input_type_str, "keyboard")) {
    return BUTTON_KEYBOARD;
  } else if (!strcmp(input_type_str, "gamepad")) {
    return BUTTON_GAMEPAD;
  } else {
    LOG(LOG_WARN, &logger_) << "'" << input_type_str
                            << "' is not a valid input type";
    return BUTTON_NULL;
  }
}

} // namespace quasi_game_engine
