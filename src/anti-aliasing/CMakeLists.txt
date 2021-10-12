cmake_minimum_required(VERSION 3.10)

project(Anti-Aliasing)

file(GLOB_RECURSE CXX_FILES
"${PROJECT_SOURCE_DIR}/*.cpp")

message(STATUS ${CXX_FILES})

# Copy the shaders folder
file(GLOB_RECURSE SHADERS "${PROJECT_SOURCE_DIR}/shaders/*")
file(MAKE_DIRECTORY shaders)
file(COPY ${SHADERS} DESTINATION "shaders")

# Copy the assets folder
file(COPY ${ASSETS_DIR} DESTINATION "${PROJECT_BINARY_DIR}")

add_executable(Anti-Aliasing ${CXX_FILES})

target_compile_options(Anti-Aliasing PUBLIC -ggdb)
target_link_libraries(Anti-Aliasing PUBLIC glad glfw glm assimp stb_image)
