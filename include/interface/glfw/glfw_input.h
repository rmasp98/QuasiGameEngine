/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_GLFW_INPUT_H
#define QGE_GLFW_INPUT_H

#include "interface/input.h"
#include "interface/action_list.h"
#include "utils/logging/logger.h"

#include <GLFW/glfw3.h>

namespace quasi_game_engine {

class GlfwInput : public Input {
/*------------------------------------------------------------------------------
  GLFW implementation of the input interface. Apart from construction, it should
  never be used directly. This class should only be created and modified by
  GlfwInterface, only get functions can be used externally
Notes
 - Should change update to a set of callbacks
------------------------------------------------------------------------------*/
  friend class GlfwInputHelper; // Allow GlfwInterface to create this class
 public:
  ~GlfwInput() final = default;

  //Getting rid of copy/move constructors/assignment operators (may need later)
  GlfwInput(const GlfwInput&) = delete;
  GlfwInput& operator=(const GlfwInput&) = delete;
  GlfwInput(GlfwInput&&) = delete;
  GlfwInput& operator=(GlfwInput&&) = delete;

  //This is static to allow action list to be able to call it
  static bool ValidButton(int button_value);

  void Update() final;
  bool IsActionActive(ActionEnum action) const final {
    return action_list_.IsActionActive(action);
  };
  const double* GetMouseMovement() const final { return diff_pos_; };
  const double* GetMousePosition() const final { return cursor_; };

 protected:
  // Pass the logger and window created by the interface, config file should detail input mapping
  GlfwInput(Logger logger, GLFWwindow* window, const char* config_file_name);

 private:
  bool IsPressed(Action action) const;

  Logger logger_;
  GLFWwindow* window_; // This has to be a pointer and is managed by GLFW
  ActionList action_list_;
  double cursor_[2], diff_pos_[2]; // This only double because of stupid glfw!
};


class GlfwInputHelper {
/*------------------------------------------------------------------------------
  Helper class that allows GlfwInterface to be able to create GlfwInput and
  delete the Input class. This is a form of access control to make sure
  GlfwInterface does not have too much access to other classes private functions
Notes
 - Should probably sit inside another file
------------------------------------------------------------------------------*/
  friend class GlfwInterface;
  // This is the only place you can create this class
  static GlfwInput* CreateGlfwInput(Logger logger, GLFWwindow* window,
                                    const char* config_file_name) {
    return new GlfwInput(logger, window, config_file_name);
  };

  // Only this and other *InputHelper's will be able to delete this class
  static void DeleteInput(Input* input) { delete input; };
};

} // namespace quasi_game_engine

#endif // QGE_GLFW_INPUT_H
