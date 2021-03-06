cmake_minimum_required(VERSION 3.10)

project(Light_Maps)

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

add_executable(Light_Maps ${CXX_FILES})

target_compile_options(Light_Maps PUBLIC -ggdb)
target_link_libraries(Light_Maps PUBLIC glad glfw glm)
