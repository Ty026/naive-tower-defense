cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BUILD_SHARED_LIBS OFF)
file(GLOB ENGINE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc")
add_library(engine STATIC ${ENGINE_SOURCES})

if(NOT EMSCRIPTEN)
  option(GLFW_BUILD_DOCS OFF)
  option(GLFW_BUILD_EXAMPLES OFF)
  option(GLFW_BUILD_TESTS OFF)
  add_subdirectory(3rdparty/glfw)
  include(3rdparty/glad.cmake)
  target_link_libraries(engine glad)
endif()
include(3rdparty/stb.cmake)

# https://github.com/ocornut/imgui/issues/2591
target_link_libraries(engine glfw stb)


target_include_directories(engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
