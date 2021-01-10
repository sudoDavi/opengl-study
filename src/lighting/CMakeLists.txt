cmake_minimum_required(VERSION 3.10)

project(Lighting)

file(GLOB_RECURSE CXX_FILES
"${PROJECT_SOURCE_DIR}/*.cpp")

message(STATUS ${CXX_FILES})

# Copy the shaders folder
file(GLOB_RECURSE SHADERS "${PROJECT_SOURCE_DIR}/shaders/*")
file(MAKE_DIRECTORY shaders)
file(COPY ${SHADERS} DESTINATION "shaders")

# Copy the assets folder
file(GLOB_RECURSE ASSETS "${PROJECT_SOURCE_DIR}/assets/*")
file(MAKE_DIRECTORY assets)
file(COPY ${ASSETS} DESTINATION "assets")

add_executable(Lighting ${CXX_FILES})

target_compile_options(Lighting PUBLIC -ggdb)
target_link_libraries(Lighting PUBLIC glad glfw glm)
