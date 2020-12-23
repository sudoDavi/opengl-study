cmake_minimum_required(VERSION 3.10)

project(Rectangle)

add_executable(Rectangle rectangle.cpp)

target_link_libraries(Rectangle PUBLIC glad glfw)
