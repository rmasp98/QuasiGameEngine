
#include "main/main.h"
#include "renderer/opengl/opengl_render_config.h"
#include "renderer/opengl/opengl_renderer.h"
#include "renderer/render_config.h"
#include "renderer/renderer.h"
#include "interface/action_list.h"
#include "interface/device_interface.h"
//This needs to be after render files due to it containing gl.h
#include "interface/glfw/glfw_interface.h"
#include "resource/resource_manager.h"
#include "resource/texture.h"
#include "utils/logging/log_worker.h"
#include "utils/logging/logger.h"
#include "utils/qge_array.h"


//temporary headers during development
#include "interface/ui/ui_test.h"
#include "utils/shader.h"


using namespace quasi_game_engine;

int main() {
  printf("%s\n", "It has begun!");

  auto log_worker = new LogWorker();
  log_worker->Init();

  DeviceInterface* interface = new GlfwInterface(
      log_worker,
      "/home/rmaspero/graphics/quasiGameEngine/config/action_map.cfg");
  const Input* input = interface->GetInput();

  Renderer* render_manager = new OpenGLRenderer(log_worker);
  render_manager->InitGraphics();

  auto ui = new UiTest(log_worker, interface->GetWindow(), render_manager);

  auto resource_manager = new ResourceManager(log_worker);

  auto picture = resource_manager->GetResource(
      Asset("/home/rmaspero/graphics/quasiGameEngine/assets/Dragon_ground_color.jpg",
      ""));
  if (picture != nullptr) picture->LoadToGraphics(render_manager);


  auto object = resource_manager->GetResource(
      Asset("/home/rmaspero/graphics/quasiGameEngine/assets/Dragon_2.5.3ds",
      "0"));
  if (object != nullptr) object->LoadToGraphics(render_manager);

  RenderConfig* main_render_config = new OpenGLRenderConfig(
      Logger("[     Renderer     ]", "logs/Renderer.log", log_worker),
      RC_DEFAULT);

  // This should be in rendermanager!
  std::vector<std::string> shaders(2, "");
  shaders[0] = "/home/rmaspero/graphics/quasiGameEngine/shaders/main.vs";
  shaders[1] = "/home/rmaspero/graphics/quasiGameEngine/shaders/main.fs";
  unsigned int program_id = LoadShaders(shaders);

  glm::vec3 pos = glm::vec3(0.0f, 0.0f, 20.0f);
  double mouse_speed = 0.03f, dt = 0.016;
  glm::vec2 angle = glm::vec2(3.14f, 0.0f);

  glViewport(0, 0, 1920, 1080);
  glm::mat4 proj = glm::perspective(glm::radians(45.0f),
                                    (float)(1920.0 / 1080.0), 0.1f, 100.0f);
  glm::mat4 view, model = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
  model = glm::rotate(model, -1.55f, glm::vec3(1.0, 0.0, 0.0));

  do {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear screen
    interface->PollEvents();  // Check for key and mouse events

    // update the game

    glUseProgram(program_id);
    main_render_config->ApplyConfig();

    if (picture != nullptr) {
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, picture->GetResource());
    }

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

    if (input->IsActionActive(ACTION_UP)) pos += facing * 0.1f;
    if (input->IsActionActive(ACTION_DOWN)) pos -= facing * 0.1f;
    if (input->IsActionActive(ACTION_LEFT)) pos += right * 0.1f;
    if (input->IsActionActive(ACTION_RIGHT)) pos -= right * 0.1f;

    view = glm::lookAt(pos, pos + facing, glm::vec3(0, 1, 0));
    glm::mat4 mvp = proj * view * model;

    // render_manager->draw();

    glUniformMatrix4fv(glGetUniformLocation(program_id, "MVP"), 1, GL_FALSE,
                       &mvp[0][0]);
    glBindVertexArray(object->GetResource());
    glDrawElements(GL_TRIANGLES, 36656 * 3, GL_UNSIGNED_INT, (void*)0);

    glBindVertexArray(0);

    ui->Update();

    // Swap second buffer
    interface->SwapBuffers();

  } while (interface->IsWindowOpen());

  delete resource_manager;
  delete render_manager;

  delete interface;

  // this should always be deleted last to ensure all logs are written
  delete log_worker;
}
