cmake_minimum_required(VERSION 2.8)
project(gbemu)
set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type")
set(CMAKE_C_COMPILER gcc     CACHE STRING "Compiler")

include(cmake/macros.cmake)

if(ANDROID)
else()
    set(ROOT ${CMAKE_CURRENT_SOURCE_DIR})

    include(cmake/options.cmake)
    include(cmake/add_source.cmake)

    if(COVERAGE)
        include(cmake/coverage.cmake)
    endif()

    add_executable(gbemu ${ALL_SRC} ${ALL_HEADER})
    #set_target_properties(gbemu PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")

    include(cmake/definitions.cmake)

    if(MINGW)
        include(cmake/mingw.cmake)
    else()
        target_link_libraries(gbemu SDL2)
    endif()
endif()
