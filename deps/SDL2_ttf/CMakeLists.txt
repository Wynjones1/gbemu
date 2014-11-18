cmake_minimum_required(VERSION 2.8)
project(SDL2_ttf)

set(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB SRC ${ROOT}/*.c)

include_directories(${ROOT}/../SDL2/include/)
include_directories(${ROOT}/external/freetype-2.4.12/include)
add_library(SDL2_ttf ${SRC})
target_link_libraries(SDL2_ttf freetype)
