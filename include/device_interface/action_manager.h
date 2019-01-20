/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/
#ifndef QGE_ACTION2_H
#define QGE_ACTION2_H

#include "utils/json_file.h"

#include <string>
#include <vector>
#include <map>
#include <unordered_set>


namespace quasi_game_engine {

// All buttons should be tied to an ActionEnum
// Any new actions must be inserted before ACTION_NULL, should be given a consequtive
// value and value of ACTION_NULL should be iterated
enum ActionEnum {
  ACTION_UP, ACTION_DOWN, ACTION_RIGHT, ACTION_LEFT,
  ACTION_ESC,
  ACTION_NULL,
  ACTION_SIZE
};

struct Action {
 public:
  Action(std::string name_in, bool is_hold_in)
    : name(name_in), is_hold(is_hold_in), bit_position(0) {};
  //TODO: need to work out why we need this?
  Action() : name(""), is_hold(false), bit_position(0) {}; 
  ~Action() = default;

  //TODO: find out why const char* breaks on static structs...
  std::string name;
  bool is_hold;
  int bit_position;
};


class ActionManager {
 public:
  ActionManager(const char* config_file);
  ~ActionManager() = default;

  void Update(std::unordered_set<int> pressed_keys);
  void LoadActions(const char* config_file);

  static bool IsActionActive(ActionEnum action);
  
 private:
  std::map<int, Action*> button_list_;
  int action_state_;
  int hold_actions_;
  JsonFile json_file_;

  void SetBitOrder();

};

} // namespace quasi_game_engine

#endif // QGE_ACTION_H
