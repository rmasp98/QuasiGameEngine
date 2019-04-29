cmake_minimum_required(VERSION 3.11)
project(quasiGameEngine)

set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE   ON)

include_directories(include)

#These are added temporarily to get the engine working
include_directories(libs)
include_directories(libs/glfw/include)

#This creates the database for autocomplete in VIM
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

enable_testing()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(warnings "-Wall -Wextra -Werror")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(warnings "/W4 /WX /EHsc")
endif()
#if (NOT CONFIGURED_ONCE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warnings}")
#        CACHE STRING "Flags used by the compiler during all build types." FORCE)
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} ${warnings}")
#        CACHE STRING "Flags used by the compiler during all build types." FORCE)
#endif()

add_subdirectory(src)
add_subdirectory(libs)
add_subdirectory(test)

add_executable(quasiGameEngine src/main/main.cc)

target_link_libraries(quasiGameEngine
	qgeDevice
	qgeRenderer
	qgeResource
	qgeUi
	qgeUtils
	imgui
	/home/rmaspero/graphics/quasiGameEngine/libs/glfw/src/glfw-build/src/libglfw.so
	GL
	GLEW
	freeimage
	assimp
	pthread
)

#set(CONFIGURED_ONCE TRUE CACHE INTERNAL
#    "A flag showing that CMake has configured at least once.")