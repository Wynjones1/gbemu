cmake_minimum_required(VERSION 2.8)
project(gbemu)

file(GLOB SRC
	"./src/*.c" "./src/*.h"
	"./src/cpu/*.c" "./src/cpu/*.h"
	"./src/cpu/instructions/*.c" "./src/cpu/instructions/*.h"
	"./src/cpu/instructions/prefix_cb/*.c" "./src/cpu/instructions/prefix_cb/*.h"
	"./src/display/*.c" "./src/display/*.h")

include_directories("./src" "./src/cpu" "./src/display")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/")

add_executable(gbemu ${SRC})

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
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${sdl_cflags} -std=c11 -g")

	target_link_libraries(gbemu ${sdl_libs})
endif()

