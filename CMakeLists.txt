cmake_minimum_required(VERSION 2.8)

if(MINGW)
    include(cmake/mingw-toolchain.cmake)
endif()

project(gbemu)
set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type")

include(cmake/macros.cmake)

if(ANDROID)
else()
    set(ROOT ${CMAKE_CURRENT_SOURCE_DIR})

    include(cmake/options.cmake)
    include(cmake/add_source.cmake)

    set(CMAKE_C_FLAGS_RELEASE -Os)
    add_executable(gbemu ${ALL_SRC} ${ALL_HEADER})

    if(COVERAGE)
        include(cmake/coverage.cmake)
    endif()
    if(MINGW)
        set(DIRECTX No)
        add_definitions(-mwindows)
        add_subdirectory(${DEPS_DIR}/SDL2)
        include_directories(${DEPS_DIR}/SDL2/include)
    endif()

    include(cmake/definitions.cmake)

    if(MINGW)
        target_link_libraries(gbemu mingw32 SDL2main SDL2-static)
    else()
        target_link_libraries(gbemu SDL2 SDL2_ttf)
    endif()
endif()
