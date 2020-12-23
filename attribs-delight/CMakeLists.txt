cmake_minimum_required(VERSION 3.10)

project(MoreAttributes)

add_executable(AttribsDelight more-attribs.cpp)

target_link_libraries(AttribsDelight PUBLIC glad glfw)
