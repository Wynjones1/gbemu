cmake_minimum_required(VERSION 2.8)
project(gbemu)

file(GLOB SRC
	"./src/*.c" "./src/*.h"
	"./src/back_end/*.c" "./src/back_end/*.h"
	"./src/back_end/instructions/*.c" "./src/back_end/instructions/*.h"
	"./src/back_end/instructions/prefix_cb/*.c" "./src/back_end/instructions/prefix_cb/*.h"
	"./src/front_end/*.c" "./src/front_end/*.h")

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
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${sdl_cflags} -std=c11 -g -Wall -Werror -Wno-missing-braces -fmax-errors=1")

	target_link_libraries(gbemu ${sdl_libs})
endif()

