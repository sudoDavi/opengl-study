cmake_minimum_required(VERSION 3.10)

project(Window)

add_executable(Window window.cpp)

target_link_libraries(Window PUBLIC glad glfw)
