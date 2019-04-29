# Comented out to prevent trying to compile every time (must find if we can add a flag)
#add_subdirectory(libs/glfw)
#include(ExternalProject)
#ExternalProject_Add(glfw
#	GIT_REPOSITORY 		https://github.com/glfw/glfw.git
#	GIT_TAG						master
#	PREFIX 						../libs/glfw
#	CMAKE_ARGS 				-DGLFW_USE_WAYLAND=ON -DCMAKE_INSTALL_PREFIX=/home/rmaspero/graphics/quasiGameEngine/libs/glfw -DBUILD_SHARED_LIBS=ON
#)

add_library(imgui
        		imgui/imgui.cc
        		imgui/imgui_demo.cc
        		imgui/imgui_draw.cc
)

# This disables warnings for imgui as we don't care about external library warnings
set_property(TARGET imgui APPEND_STRING PROPERTY COMPILE_FLAGS " -w")
