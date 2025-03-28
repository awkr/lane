cmake_minimum_required(VERSION 3.26)
project(lane)

set(CMAKE_CXX_STANDARD 23)

add_executable(lane main.cpp)

add_subdirectory(third_party)

target_compile_definitions(lane PUBLIC GLFW_INCLUDE_NONE)
target_link_libraries(lane PUBLIC imgui) # glfw glad 已经在 imgui 中链接了
