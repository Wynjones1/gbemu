cmake_minimum_required(VERSION 2.8)
project(gbemu)

set(ROOT ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB SRC
	"${ROOT}/src/*.cpp"
	"${ROOT}/src/instructions/*.cpp"
	"${ROOT}/src/instructions/prefix_cb/*.cpp"
	"${ROOT}/src/testing/*.cpp")

file(GLOB HDR
	"${ROOT}/src/*.h"
	"${ROOT}/src/instructions/*.h"
	"${ROOT}/src/instructions/prefix_cb/*.h"
	"${ROOT}/src/testing/*.h")

include_directories("./src" "./src/back_end" "./src/front_end")

set(CMAKE_BUILD_TYPE "Debug" CACHE BOOL "Build type")

add_executable(gbemu ${SRC})

#set(CMAKE_CXX_FLAGS "-fpermissive ${CMAKE_CXX_FLAGS}")

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	add_definitions(-DDEBUG)
endif()

if(MSVC)
else()
	find_program(SDL_CONFIG sdl2-config)

	execute_process(COMMAND "${SDL_CONFIG}" "--libs"
					OUTPUT_VARIABLE sdl_libs
					OUTPUT_STRIP_TRAILING_WHITESPACE)
	execute_process(COMMAND ${SDL_CONFIG} "--cflags"
					OUTPUT_VARIABLE sdl_cflags
					OUTPUT_STRIP_TRAILING_WHITESPACE)

	message(STATUS "SDL Libs:  " ${sdl_libs})
	message(STATUS "SDL Flags: " ${sdl_cflags})
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${sdl_cflags} -std=c11 -g -O3 -Wall -Werror -Wno-missing-braces -Wno-unused-variable -Wno-unused")
	target_link_libraries(gbemu ${sdl_libs} pthread)
	target_link_libraries(gbemu ${sdl_libs} SDL2_ttf)
endif()

