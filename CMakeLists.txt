cmake_minimum_required(VERSION 2.8)
if(MINGW)
    include(toolchain.cmake)
endif()
project(gbemu)

set(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(DEPS ${ROOT}/deps)

file(GLOB SRC	              "${ROOT}/src/*.c")
file(GLOB SRC_INSTRUCTIONS    "${ROOT}/src/instructions/*.c")
file(GLOB SRC_PREFIX_CB       "${ROOT}/src/instructions/prefix_cb/*.c")

file(GLOB HEADER	          "${ROOT}/src/*.h")
file(GLOB HEADER_INSTRUCTIONS "${ROOT}/src/instructions/*.h")
file(GLOB HEADER_PREFIX_CB    "${ROOT}/src/instructions/prefix_cb/*.h")

source_group(headers                          FILES ${HEADER})
source_group(headers\\instructions            FILES ${HEADER_INSTRUCTIONS})
source_group(headers\\instructions\\prefix_cb FILES ${HEADER_PREFIX_CB})

source_group(src                          FILES ${SRC})
source_group(src\\instructions            FILES ${SRC_INSTRUCTIONS})
source_group(src\\instructions\\prefix_cb FILES ${SRC_PREFIX_CB})

if(TESTING)
file(GLOB SRC_TESTING         "${ROOT}/src/testing/*.c")
file(GLOB HEADER_TESTING      "${ROOT}/src/testing/*.h")
source_group(headers\\testing  FILES ${HEADER_TESTING})
source_group(src\\testing      FILES ${SRC_TESTING})
endif()

include_directories("./src" "./src/back_end" "./src/front_end")

add_executable(gbemu ${SRC} ${SRC_INSTRUCTIONS} ${SRC_PREFIX_CB} ${SRC_TESTING}
					 ${HEADER} ${HEADER_INSTRUCTIONS} ${HEADER_PREFIX_CB} ${HEADER_TESTING})

macro(DEFADD NAME DEFAULT DESC)
	set(${NAME} ${DEFAULT} CACHE BOOL ${DESC})
	if(${NAME})
		add_definitions(-D${NAME}=1)
	else()
		add_definitions(-D${NAME}=0)
	endif()
endmacro()

macro(ADD_CFLAG arg)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${arg}")
	set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} ${arg}")
endmacro()

set(CMAKE_BUILD_TYPE "Debug"    CACHE STRING "Build type")
set(32BIT                 Yes   CACHE BOOL   "Build in 32 bit mode")
set(SDL_INCLUDE_DIR       "/home/stuart/C/SDL2-MinGW/include")
DEFADD(OUTPUT_WARNINGS    Yes  "Enable warning output.")
DEFADD(OUTPUT_ERRORS      Yes  "Enable error output.")
DEFADD(OUTPUT_OUTPUT      No   "Enable stdout  output.")
DEFADD(TESTING            No   "Run tests.")
DEFADD(REGISTER_WINDOW    No   "Enable GB register window")
DEFADD(OUTPUT_DEBUG_FILES No   "Output debug output files")
DEFADD(DEBUG_WINDOW       No   "Window with current instruction and register values")
DEFADD(SPINLOCK           No   "Use busy wait to framelimit.")
DEFADD(SDLTTF             No   "Have SDL TTF")

if(${TESTING})
	add_definitions("-DCPU_DUMMY_IO=1")
else()
	add_definitions("-DCPU_DUMMY_IO=0")
endif()

if(SDL_INCLUDE_DIR)
    include_directories(SYSTEM ${SDL_INCLUDE_DIR})
endif()


find_package(Threads)
target_link_libraries(gbemu ${CMAKE_THREAD_LIBS_INIT})


if(NOT MINGW)
    find_program(SDL_CONFIG sdl2-config)
    execute_process(COMMAND "${SDL_CONFIG}" "--libs"
                    OUTPUT_VARIABLE sdl_libs
                    OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND ${SDL_CONFIG} "--cflags"
                    OUTPUT_VARIABLE sdl_cflags
                    OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()

add_definitions(--std=c99)
add_definitions(-Wno-unused-variable -Wno-unused-function)
add_definitions(-Werror -Wall -Wextra)
add_definitions(-Wno-unused-result -Wno-unused-parameter)

if(MINGW)
    set(MINGW_DIR ./SDL2/i686-w64-mingw32/)
    add_definitions(-mwindows)
    include_directories(${MINGW_DIR}/include)
    find_library(libSDL2     SDL2 ${MINGW_DIR}/lib)
    find_library(libSDL2main SDL2main ${MINGW_DIR}/lib)
    target_link_libraries(gbemu mingw32 ${libSDL2main} ${libSDL2})
else()
    target_link_libraries(gbemu SDL2)
endif()

