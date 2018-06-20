#include "interface/glfw/glfw_interface.h"

#include "utils/logging/logger.h"
#include "utils/logging/log_worker.h"

namespace quasi_game_engine {

GlfwInterface::GlfwInterface(LogWorker* log_worker, const char* config_file_name)
    : logger_("[Interface  Manager]", "logs/DeviceInterfaceManager.log",
              log_worker) {
  LOG(LOG_INFO, &logger_) << "Initialising GLFW";
  if (!glfwInit()) {
    LOG(LOG_FATAL, &logger_) << "Failed to initialise GLFW";
  }

  // Need to read this in from config file
  //glfwWindowHint(GLFW_SAMPLES, 4); //Antialiasing = 4 samples
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Pointing to version of openGL
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Should find out about other profiles

  //Enable mutlisampling Antialiasing
  // glfwWindowHint(GLFW_SAMPLES, 4);
  // glEnable(GL_MULTISAMPLE);

  CreateWindow("Quasi Game Engine", 1920, 1080, false);

  glfwMakeContextCurrent(window_); //Makes this window the current window

  //TODO: Could make input constructor dump and then initialise it here
  input_ = GlfwInputHelper::CreateGlfwInput(logger_, window_, config_file_name);

};


GlfwInterface::~GlfwInterface() {
  GlfwInputHelper::DeleteInput(input_);
  glfwTerminate();
}


void GlfwInterface::CreateWindow(const char* name, int width, int height,
                                 bool full_screen) {
  LOG(LOG_INFO, &logger_) << "Creating GLFW window";
  if (full_screen) {
    //Gets information about the primary monitor. TODO: is this actually necessary
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    //Creates the window
    window_ = glfwCreateWindow(width, height, name, monitor, nullptr);
  } else {
    window_ = glfwCreateWindow(width, height, name, nullptr, nullptr);
  }

  //Checks to ensure a window was created properly
  if (window_ == nullptr) {
    LOG(LOG_FATAL, &logger_) << "Failed to open GLFW window";
  }
}


void GlfwInterface::SwapBuffers() {
  glfwSwapBuffers(window_);
}


void GlfwInterface::PollEvents() {
  glfwPollEvents();
}


bool GlfwInterface::IsWindowOpen() const {
  //This is tempoary and should be changed later
  return !input_->IsActionActive(ACTION_ESC) &&
         glfwWindowShouldClose(window_) == 0;
}


void GlfwInterface::Update() {

  //May need to update window

  input_->Update();

}


//TODO: This needs to be set at some point
//void SetApiVersion(short major, short minor, bool isCore) {}


}  // namespace quasi_game_engine
