cmake_minimum_required(VERSION 3.10)

project(CUbemaps)

file(GLOB_RECURSE CXX_FILES
"${PROJECT_SOURCE_DIR}/*.cpp")

message(STATUS ${CXX_FILES})

# Copy the shaders folder
file(GLOB_RECURSE SHADERS "${PROJECT_SOURCE_DIR}/shaders/*")
file(MAKE_DIRECTORY shaders)
file(COPY ${SHADERS} DESTINATION "shaders")

# Copy the assets folder
file(COPY ${ASSETS_DIR} DESTINATION "${PROJECT_BINARY_DIR}")

add_executable(Cubemaps ${CXX_FILES})

target_compile_options(Cubemaps PUBLIC -ggdb)
target_link_libraries(Cubemaps PUBLIC glad glfw glm assimp stb_image)
