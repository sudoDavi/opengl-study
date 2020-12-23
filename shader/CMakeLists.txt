cmake_minimum_required(VERSION 3.10)

project(Shaders)

file(GLOB_RECURSE CXX_FILES
"${PROJECT_SOURCE_DIR}/*.cpp")

message(STATUS ${CXX_FILES})

add_executable(Shaders ${CXX_FILES})

target_link_libraries(Shaders PUBLIC glad glfw)
