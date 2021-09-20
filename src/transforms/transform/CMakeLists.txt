cmake_minimum_required(VERSION 3.10)

project(Transforms)

file(GLOB_RECURSE CXX_FILES
"${PROJECT_SOURCE_DIR}/*.cpp")

message(STATUS ${CXX_FILES})

# Copy the shaders folder
file(GLOB_RECURSE SHADERS "${PROJECT_SOURCE_DIR}/shaders/*")
file(MAKE_DIRECTORY shaders)
file(COPY ${SHADERS} DESTINATION "shaders")

# Copy the assets folder
file(COPY ${ASSETS_DIR} DESTINATION "${PROJECT_BINARY_DIR}")

add_executable(Transforms ${CXX_FILES})

target_compile_options(Transforms PUBLIC -ggdb)
target_link_libraries(Transforms PUBLIC glad glfw glm)
