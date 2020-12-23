cmake_minimum_required(VERSION 3.10)

project(Uniforms)

add_executable(Uniforms uniforms.cpp)

target_link_libraries(Uniforms PUBLIC glad glfw)
