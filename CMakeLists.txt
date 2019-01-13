cmake_minimum_required(VERSION 3.11)
project(quasiGameEngine)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

#These are added temporarily to get the engine working
include_directories(libs)

add_executable(quasiGameEngine
        src/interface/glfw/glfw_input.cc
        src/interface/glfw/glfw_interface.cc
        src/interface/action_manager.cc
        src/ui/user_interface.cc
        src/main/main.cc
        src/renderer/opengl/opengl_renderer.cc
        src/renderer/opengl/opengl_shader.cc
#        src/renderer/opengl/opengl_render_queue.cc
        src/resource/mesh.cc
        src/resource/resource_base.cc
        src/resource/image_loader.cc
        src/resource/object_loader.cc
        src/resource/resource_file_manager.cc
        src/resource/resource_manager.cc
        src/resource/texture.cc
        src/utils/json_file.cc
        src/utils/logging/log_worker.cc
        src/utils/logging/log_capture.cc
        src/utils/file_utils.cc
        src/utils/memory_manager.cc
#        test/interface/device_interface_test.cc

				#Should be moved out into a seperate library dependency
        libs/imgui/imgui.cc
        libs/imgui/imgui_demo.cc
        libs/imgui/imgui_draw.cc
        )

TARGET_LINK_LIBRARIES(quasiGameEngine GL glfw GLEW freeimage assimp pthread)
