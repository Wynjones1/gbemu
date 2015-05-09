cmake_minimum_required(VERSION 2.8)
project(SDL2_ttf)

include_directories(
    ${TTF_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIRS})
set(SOURCE SDL_ttf.c showfont.c glfont.c)
add_library(SDL_ttf ${SOURCE})
