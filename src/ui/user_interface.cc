/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#include "ui/user_interface.h"
#include "imgui/imgui.h"

namespace quasi_game_engine {

UserInterface::UserInterface(Renderer* render_manager) { 
  //initialise imgui
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

  // Setup style
  ImGui::StyleColorsDark();

  uiRenderConfig = new OpenGLRenderConfig(RC_IMGUI);

  // We can honor GetMouseCursor() values (optional)
  io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
  // We can honor io.WantSetMousePos requests (optional, rarely used)   
  io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;    

  std::vector<std::string> shaders(2, "");
  shaders[0] = "/home/rmaspero/graphics/quasiGameEngine/shaders/ui.vs";
  shaders[1] = "/home/rmaspero/graphics/quasiGameEngine/shaders/ui.fs";
  shader = render_manager->LoadShaders(shaders);

  // Manually load and put font texture into imgui
  unsigned char* pixels;
  int width, height;
  // Load as RGBA 32-bits (75% of the memory is wasted, but default font is so small) because it is more likely to be compatible with user's existing shaders. If your ImTextureId represent a higher-level concept than just a GL texture id, consider calling GetTexDataAsAlpha8() instead to save on GPU memory.
  io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);   

  //TODO: Need to delete this 
  int tex_id;
  render_manager->LoadImage(pixels, width, height, false, &tex_id);

  io.Fonts->TexID = (void *)(intptr_t)tex_id;
}

UserInterface::~UserInterface() {
  //destroy imgui
  ImGui::DestroyContext();
}


void UserInterface::Update(std::shared_ptr<Input> input, Renderer* render_manager) {
  ImGuiIO& io = ImGui::GetIO();
  input->UpdateGuiIO(&io);

  // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
  std::vector<int> buffer_size {
    (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x),
    (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y)
  };
  if (buffer_size[0] == 0 || buffer_size[1] == 0)
      return;

  //imgui new frame (may want to do more setting up here)
  ImGui::NewFrame();

  //define windows`
  bool show_demo_window_ = true;
  if (show_demo_window_) {
    //ImGui::SetNextWindowPos(ImVec2(65, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
    ImGui::ShowDemoWindow(&show_demo_window_);
  }

  //render ui?
  ImGui::Render();
  // Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, polygon fill
  // This can fail but will sort later
  if (uiRenderConfig != nullptr)
    uiRenderConfig->ApplyConfig();
  
  ImDrawData* draw_data = ImGui::GetDrawData();
  draw_data->ScaleClipRects(io.DisplayFramebufferScale);

  shader->SetActive();

  std::vector<AttributeMetadata> metadata = {
      AttributeMetadata("Position", 2, GL_FLOAT, IM_OFFSETOF(ImDrawVert, pos), false),
      AttributeMetadata("UV", 2, GL_FLOAT, IM_OFFSETOF(ImDrawVert, uv), false),
      AttributeMetadata("Color", 4, GL_UNSIGNED_BYTE, IM_OFFSETOF(ImDrawVert, col), true)
  };

  shader->SetAttributePointer(metadata, sizeof(ImDrawVert), &vao_id);

  const std::vector<float> ortho_projection =
  {
      2.0f/io.DisplaySize.x, 0.0f,                   0.0f, 0.0f,
      0.0f,                  2.0f/-io.DisplaySize.y, 0.0f, 0.0f,
      0.0f,                  0.0f,                  -1.0f, 0.0f,
      -1.0f,                 1.0f,                   0.0f, 1.0f,
  };
  shader->SetUniformMatrix4("ProjMtx", ortho_projection);
  shader->SetUniformInteger("Texture", 0);

  render_manager->DrawImGui(draw_data, buffer_size, &vbo_id, &index_id);
}

}
