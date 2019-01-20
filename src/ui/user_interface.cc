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

std::vector<AttributeMetadata> metadata = {
    AttributeMetadata(ATTR_POSITION, 2, GL_FLOAT, IM_OFFSETOF(ImDrawVert, pos),
                      false),
    AttributeMetadata(ATTR_UV, 2, GL_FLOAT, IM_OFFSETOF(ImDrawVert, uv), false),
    AttributeMetadata(ATTR_COLOUR, 4, GL_UNSIGNED_BYTE,
                      IM_OFFSETOF(ImDrawVert, col), true)};

UserInterface::UserInterface(Renderer* render_manager,
                             std::shared_ptr<Input> input)
    : input_(input) {
  // initialise imgui
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard
  // Controls io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable
  // Gamepad Controls

  ImGui::StyleColorsDark();  // Setup style

  io.BackendFlags |=
      ImGuiBackendFlags_HasMouseCursors;  // We can honor GetMouseCursor()
                                          // values (optional)
  io.BackendFlags |=
      ImGuiBackendFlags_HasSetMousePos;  // We can honor io.WantSetMousePos
                                         // requests (optional, rarely used)

  // Manually load and put font texture into imgui
  unsigned char* pixels;
  int width, height;
  // Load as RGBA 32-bits (75% of the memory is wasted, but default font is so
  // small) because it is more likely to be compatible with user's existing
  // shaders. If your ImTextureId represent a higher-level concept than just a
  // GL texture id, consider calling GetTexDataAsAlpha8() instead to save on GPU
  // memory.
  io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

  // TODO: Need to delete this id later
  int tex_id;
  render_manager->LoadImage(pixels, width, height, false, &tex_id);

  io.Fonts->TexID = (void*)(intptr_t)tex_id;

  input_->InitGuiIO(&io);
}

UserInterface::~UserInterface() {
  // Destroy all input reference
  input_->DestroyGuiIO();

  // destroy imgui
  ImGui::DestroyContext();
}

void UserInterface::Update(std::shared_ptr<Input> input,
                           Renderer* render_manager) {
  ImGuiIO& io = ImGui::GetIO();

  std::vector<int> buffer_size{
      (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x),
      (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y)};
  // Avoid rendering when minimized, scale coordinates for retina displays
  // (screen coordinates != framebuffer coordinates)
  if (buffer_size[0] == 0 || buffer_size[1] == 0) return;

  ImGui::NewFrame();  // imgui new frame (may want to do more setting up here)

  // Update demo window
  bool show_demo_window_ = true;
  if (show_demo_window_) {
    ImGui::SetNextWindowPos(
        ImVec2(65, 20),
        ImGuiCond_FirstUseEver);  // Normally user code doesn't need/want to
                                  // call this because positions are saved in
                                  // .ini file anyway. Here we just want to make
                                  // the demo initial state a bit more friendly!
    ImGui::ShowDemoWindow(&show_demo_window_);
  }

  ImGui::Begin("Hello");
  ImGui::End();

  LoadToGraphics(render_manager, buffer_size[1]);
}

void UserInterface::LoadToGraphics(Renderer* render_manager,
                                   int buffer_height) {
  ImGuiIO& io = ImGui::GetIO();

  ImGui::Render();  // Calculate graphics information
  // Setup render state: alpha-blending enabled, no face culling, no depth
  // testing, scissor enabled, polygon fill

  ImDrawData* draw_data = ImGui::GetDrawData();
  draw_data->ScaleClipRects(io.DisplayFramebufferScale);

  // TODO: figure out how to delete these buffers
  if (draw_data->CmdListsCount > buffer_ids.size()) {
    buffer_ids.resize(draw_data->CmdListsCount, BufferIds(0, 0, 0));
  }

  DrawConfig ui_config;
  ui_config.mvp = {
      2.0f / io.DisplaySize.x,
      0.0f,
      0.0f,
      0.0f,
      0.0f,
      2.0f / -io.DisplaySize.y,
      0.0f,
      0.0f,
      0.0f,
      0.0f,
      -1.0f,
      0.0f,
      -1.0f,
      1.0f,
      0.0f,
      1.0f,
  };

  for (int n = 0; n < draw_data->CmdListsCount; ++n) {
    const ImDrawList* cmd_list = draw_data->CmdLists[n];
    render_manager->LoadImGui(cmd_list, metadata, &buffer_ids[n]);

    unsigned short* idx_buffer_offset = 0;
    for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; ++cmd_i) {
      const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
      if (pcmd->UserCallback) {
        pcmd->UserCallback(cmd_list, pcmd);
      } else {
        ui_config.vao = buffer_ids[n].vao;
        ui_config.num_elements = pcmd->ElemCount;
        ui_config.offset = idx_buffer_offset;
        ui_config.texture_id = (intptr_t)pcmd->TextureId;
        ui_config.scissor_box = {(int)pcmd->ClipRect.x,
                                 (int)(buffer_height - pcmd->ClipRect.w),
                                 (int)(pcmd->ClipRect.z - pcmd->ClipRect.x),
                                 (int)(pcmd->ClipRect.w - pcmd->ClipRect.y)};

        render_manager->PushToRenderQueue(DP_IMGUI, ui_config);
      }

      idx_buffer_offset += pcmd->ElemCount;
    }
  }
}

}  // namespace quasi_game_engine
