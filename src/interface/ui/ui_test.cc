
#include "interface/ui/ui_test.h"
#include "interface/ui/imgui/imgui.h"

#include "interface/ui/imgui/imgui_impl_glfw_gl3.h"

//#include "renderer/renderManager.h"


//temp libraries
// #include <GL/glew.h>
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <GLFW/glfw3.h>


namespace quasi_game_engine {

   UiTest::UiTest(LogWorker *log_worker, GLFWwindow *window, RenderManager* render_manager)
                : logger_("[    UI  Manager   ]", "logs/UI.log", log_worker),
                  window_(window) {

      //initialise imgui
      ImGui::CreateContext();
      ImGuiIO& io = ImGui::GetIO(); (void)io;
      //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
      //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
      ImGui_ImplGlfwGL3_Init(window_, true, render_manager);

      // Setup style
      ImGui::StyleColorsDark();

      show_demo_window_ = true;


   }

   UiTest::~UiTest() {

      //destroy imgui
      ImGui_ImplGlfwGL3_Shutdown();
      ImGui::DestroyContext();

   }


   void UiTest::Update() {

      //imgui new frame (may want to do more setting up here)
      ImGui_ImplGlfwGL3_NewFrame();

      //define windows`
      if (show_demo_window_) {
          ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
          ImGui::ShowDemoWindow(&show_demo_window_);
      }

      //render ui?
      ImGui::Render();
      ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

   }

}
