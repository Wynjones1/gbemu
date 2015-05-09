cmake_minimum_required(VERSION 2.8)

list(APPEND CMAKE_MODULE_PATH
            ${CMAKE_SOURCE_DIR}/cmake
            ${CMAKE_SOURCE_DIR}/deps)
if(MINGW)
    include(mingw-toolchain)
endif()

project(gbemu)
find_package(SDL2)
find_package(TTF)
find_package(SDL2_ttf)

set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type")

include(macros)
include(coverage)

set(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_C_FLAGS_RELEASE -Os)
set(CMAKE_C_FLAGS --std=c99)

#################################################################
#                          Definitions 
#################################################################


#################################################################
#                            Options 
#################################################################
set(COVERAGE               No    CACHE BOOL   "Generate coverage data")
set(CHECK                  No    CACHE BOOL   "add everything option")
DEFADD(AUDIO               Yes  "Build with audio")
DEFADD(OUTPUT_WARNINGS     Yes  "Enable warning output.")
DEFADD(OUTPUT_ERRORS       Yes  "Enable error output.")
DEFADD(OUTPUT_OUTPUT       No   "Enable stdout  output.")
DEFADD(TESTING             No   "Run tests.")
DEFADD(REGISTER_WINDOW     No   "Enable GB register window")
DEFADD(OUTPUT_DEBUG_FILES  No   "Output debug output files")
DEFADD(DEBUG_WINDOW        Yes  "Window with current instruction and register values")
DEFADD(SPINLOCK            No   "Use busy wait to framelimit.")
DEFADD(INTRO_ONLY          No   "Only run the nintendo intro.")
DEFADD(EMBEDDED_FILES      Yes  "Embbed all files in binary")
DEFADD(CONTROLS            No   "Display GameBoy controls")
DEFADD(LOG_INSTRUCTIONS    No   "Log each instruction executed")
DEFADD(BUILD_SDL           Yes  "Build SDL2 from source.")
DEFADD(BUILD_TTF           Yes  "Build TTF  from source.")
DEFADD(HAVE_TTF            Yes  "Have TTF Support")

set(DEPS_DIR               ${CMAKE_SOURCE_DIR}/deps CACHE INTERNAL "")

add_subdirectory(src)

if(MINGW)
    set(DIRECTX No)
    add_definitions(-mwindows)
endif()

if(BUILD_SDL OR MINGW)
    add_subdirectory(${DEPS_DIR}/SDL2)
endif()

if(BUILD_TTF OR MINGW)
    add_subdirectory(${DEPS_DIR}/freetype-2.5.5)
    add_subdirectory(${DEPS_DIR}/SDL2_ttf)
endif()

