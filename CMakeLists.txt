cmake_minimum_required(VERSION 2.8)
project(gbemu)

file(GLOB SRC
	"./src/*.c" "./src/*.h"
	"./src/instructions/*.c" "./src/instructions/*.h"
	"./src/instructions/prefix_cb/*.c" "./src/instructions/prefix_cb/*.h"
	"./src/testing/*.c" "./src/testing/*.h")

include_directories("./src" "./src/back_end" "./src/front_end")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/")
set(CMAKE_BUILD_TYPE "Debug" CACHE BOOL "Build type")

add_executable(gbemu ${SRC})

macro(DEFADD NAME DEFAULT DESC)
	set(${NAME} ${DEFAULT} CACHE BOOL ${DESC})
	if(${NAME})
		add_definitions(-D${NAME}=1)
	else()
		add_definitions(-D${NAME}=0)
	endif()

endmacro()
macro(ADD_CFLAG arg)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${arg}")
endmacro()


set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type")
set(32BIT Yes CACHE BOOL "Build in 32 bit mode")
set(BUILD_DEPS No CACHE BOOL "Build dependancies from source")
DEFADD(OUTPUT_WARNINGS Yes "Enable warning output.")
DEFADD(OUTPUT_ERRORS Yes "Enable warning output.")
DEFADD(OUTPUT_OUTPUT No "Enable stdout output.")
DEFADD(DISPLAY_ENABLED Yes "Use display.")
DEFADD(DISPLAY_THREAD Yes "Display drawing on separate thread.")
DEFADD(TESTING No "Run tests.")
DEFADD(AUDIO_ENABLED No "Enable Audio")
DEFADD(REGISTER_WINDOW Yes "Enable GB register window")

if(${TESTING})
	add_definitions("-DCPU_DUMMY_IO=1")
else()
	add_definitions("-DCPU_DUMMY_IO=0")
endif()
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	add_definitions(-DDEBUG=1)
else()
	add_definitions(-DDEBUG=0)
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
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${sdl_cflags} -std=c11 -g -Wall -Werror -Wno-missing-braces -Wno-unused-variable -Wno-unused")
	target_link_libraries(gbemu ${sdl_libs} pthread)
	target_link_libraries(gbemu ${sdl_libs} SDL2_ttf)
endif()

