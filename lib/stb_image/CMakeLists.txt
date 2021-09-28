project(stb_image)

#include_directories(include)
add_library(stb_image STATIC stb_image.cpp)
target_include_directories(stb_image PUBLIC ./)
