/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#include "main/main.h"
#include "renderer/opengl/opengl_renderer.h"
#include "renderer/renderer.h"
// These have to be after renderer includes
#include "device_interface/action_manager.h"
#include "device_interface/device_interface.h"
#include "device_interface/glfw/glfw_interface.h"
#include "resource/resource_manager.h"
#include "resource/texture.h"
#include "ui/user_interface.h"
#include "utils/logging/log_capture.h"

using namespace quasi_game_engine;

int main() {
  LOG(INFO, GENERAL) << "It has begun!";

  DeviceInterface* interface = new GlfwInterface(
      "/home/rmaspero/graphics/quasiGameEngine/config/action_map.cfg");
  auto input = interface->GetInput();

  Renderer* render_manager = new OpenGLRenderer();

  // This sets up the UI part
  auto ui = new UserInterface(render_manager, input);

  auto resource_manager = new ResourceManager();

  auto picture = resource_manager->GetResource(Asset(
      "/home/rmaspero/graphics/quasiGameEngine/assets/Dragon_ground_color.jpg",
      ""));
  if (picture != nullptr) picture->LoadToGraphics(render_manager);

  auto object = resource_manager->GetResource(Asset(
      "/home/rmaspero/graphics/quasiGameEngine/assets/Dragon_2.5.3ds", "0"));
  if (object != nullptr) object->LoadToGraphics(render_manager);

  glm::vec3 pos = glm::vec3(0.0f, 0.0f, 20.0f);
  double mouse_speed = 0.03f, dt = 0.016;
  glm::vec2 angle = glm::vec2(3.14f, 0.0f);

  glm::mat4 proj = glm::perspective(glm::radians(45.0f),
                                    (float)(1920.0 / 1080.0), 0.1f, 100.0f);
  glm::mat4 view, model = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
  model = glm::rotate(model, -1.55f, glm::vec3(1.0, 0.0, 0.0));

  do {
    interface->PollEvents();  // Check for key and mouse events

    // update the game
    interface->Update();

    // Determines the angle of the camera based on how far the cursor position
    // has changed
    const double* mouse_diff = input->GetMouseMovement();
    angle.x += mouse_speed * dt * mouse_diff[0];
    double d_theta = mouse_speed * dt * mouse_diff[1];

    // Makes sure that camera can't look up or down too far
    if (angle.y + d_theta <= -0.9 * glm::pi<GLfloat>() / 2.0f)
      angle.y = -0.9 * glm::pi<GLfloat>() / 2.0f;
    else if (angle.y + d_theta > 0.9 * glm::pi<GLfloat>() / 2.0f)
      angle.y = 0.9 * glm::pi<GLfloat>() / 2.0f;
    else
      angle.y += d_theta;

    // Generates a vector for the facing direction from the angles
    glm::vec3 facing = glm::vec3(cos(angle.y) * sin(angle.x), sin(angle.y),
                                 cos(angle.y) * cos(angle.x));

    // Creates the movement (2D) vectors (posible should normalise)
    // this is for 2d movement
    // glm::vec3 dir = glm::vec3(facing.x, 0, facing.z);
    glm::vec3 right = glm::vec3(facing.z, 0, -facing.x);

    if (ActionManager::IsActionActive(ACTION_UP)) pos += facing * 0.1f;
    if (ActionManager::IsActionActive(ACTION_DOWN)) pos -= facing * 0.1f;
    if (ActionManager::IsActionActive(ACTION_LEFT)) pos += right * 0.1f;
    if (ActionManager::IsActionActive(ACTION_RIGHT)) pos -= right * 0.1f;

    view = glm::lookAt(pos, pos + facing, glm::vec3(0, 1, 0));
    glm::mat4 mvp = proj * view * model;
    std::vector<float> mvp_vec = {mvp[0][0], mvp[0][1], mvp[0][2], mvp[0][3],
                                  mvp[1][0], mvp[1][1], mvp[1][2], mvp[1][3],
                                  mvp[2][0], mvp[2][1], mvp[2][2], mvp[2][3],
                                  mvp[3][0], mvp[3][1], mvp[3][2], mvp[3][3]};

    DrawConfig dragon_draw;
    dragon_draw.vao = object->GetResource();
    dragon_draw.mvp = mvp_vec;
    dragon_draw.num_elements = 36656 * 3;
    dragon_draw.offset = 0;
    dragon_draw.texture_id = picture->GetResource();
    render_manager->PushToRenderQueue(DP_DEFAULT, dragon_draw);

    ui->Update(input, render_manager);

    if (true) {
      render_manager->Draw();
      interface->SwapBuffers();  // Swap to second buffer
    } else {
      interface->SwapBuffers();  // Swap to second buffer
      render_manager->Draw();
    }
  } while (interface->IsWindowOpen());

  delete resource_manager;
  delete render_manager;
  printf("What!\n");

  delete interface;
}
