cmake_minimum_required(VERSION 3.10)

project(Triangles-Exercise)

add_executable(Triangles triangles.cpp)

target_link_libraries(Triangles PUBLIC glad glfw)
