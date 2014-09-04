cmake_minimum_required(VERSION 2.8)
project(gbemu)

file(GLOB SRC
	"./src/*.c" "./src/*.h"
	"./src/instructions/*.c" "./src/instructions/*.h"
	"./src/instructions/prefix_cb/*.c" "./src/instructions/prefix_cb/*.h"
	"./src/testing/*.c" "./src/testing/*.h")

include_directories("./src" "./src/back_end" "./src/front_end")

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
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${sdl_cflags} -std=c11 -g -O0 -pg -Wall -Werror -Wno-missing-braces -Wno-unused-variable -Wno-unused")
	target_link_libraries(gbemu ${sdl_libs} pthread)
	target_link_libraries(gbemu ${sdl_libs} SDL2_ttf)
endif()

