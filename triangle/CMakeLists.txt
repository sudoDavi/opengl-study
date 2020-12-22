cmake_minimum_required(VERSION 3.10)

project(Triangle)

add_executable(Triangle triangle.cpp)

target_link_libraries(Triangle PUBLIC glad glfw)
