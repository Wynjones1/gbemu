cmake_minimum_required(VERSION 2.8)
set(CMAKE_C_COMPILER gcc CACHE STRING "Compiler")
project(gbemu)

include(cmake/macros.cmake)

set(ROOT ${CMAKE_CURRENT_SOURCE_DIR})

include(cmake/options.cmake)
include(cmake/add_source.cmake)

if(COVERAGE)
    include(cmake/coverage.cmake)
endif()

add_executable(gbemu ${ALL_SRC} ${ALL_HEADER})

include(cmake/definitions.cmake)

if(MINGW)
    include(cmake/mingw.cmake)
else()
    target_link_libraries(gbemu SDL2)
endif()

