cmake_minimum_required(VERSION 2.8)
project(gbemu)

set(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(DEPS ${ROOT}/deps)

file(GLOB SRC	              "${ROOT}/src/*.c")
file(GLOB SRC_INSTRUCTIONS    "${ROOT}/src/instructions/*.c")
file(GLOB SRC_PREFIX_CB       "${ROOT}/src/instructions/prefix_cb/*.c")
file(GLOB SRC_TESTING         "${ROOT}/src/testing/*.c")

file(GLOB HEADER	          "${ROOT}/src/*.h")
file(GLOB HEADER_INSTRUCTIONS "${ROOT}/src/instructions/*.h")
file(GLOB HEADER_PREFIX_CB    "${ROOT}/src/instructions/prefix_cb/*.h")
file(GLOB HEADER_TESTING      "${ROOT}/src/testing/*.h")

source_group(headers                          FILES ${HEADER})
source_group(headers\\instructions            FILES ${HEADER_INSTRUCTIONS})
source_group(headers\\instructions\\prefix_cb FILES ${HEADER_PREFIX_CB})
source_group(headers\\testing                 FILES ${HEADER_TESTING})

source_group(src                          FILES ${SRC})
source_group(src\\instructions            FILES ${SRC_INSTRUCTIONS})
source_group(src\\instructions\\prefix_cb FILES ${SRC_PREFIX_CB})
source_group(src\\testing                 FILES ${SRC_TESTING})

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

set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type")
set(32BIT                 Yes   CACHE BOOL "Build in 32 bit mode")
set(BUILD_DEPS            No    CACHE BOOL "Build dependancies from source")
DEFADD(OUTPUT_WARNINGS    Yes  "Enable warning output.")
DEFADD(OUTPUT_ERRORS      Yes  "Enable error output.")
DEFADD(OUTPUT_OUTPUT      No   "Enable stdout  output.")
DEFADD(TESTING            No   "Run tests.")
DEFADD(REGISTER_WINDOW    NO   "Enable GB register window")
DEFADD(OUTPUT_DEBUG_FILES No   "Output debug output files")
DEFADD(DEBUG_WINDOW       No   "Window with current instruction and register values")

if(${TESTING})
	add_definitions("-DCPU_DUMMY_IO=1")
else()
	add_definitions("-DCPU_DUMMY_IO=0")
endif()

include_directories(${DEPS}/SDL2/include)
include_directories(${DEPS}/SDL2_ttf/)
add_definitions(--std=c11)
add_definitions(-Wno-unused-variable -Wno-unused-function)

find_package(Threads)
target_link_libraries(gbemu ${CMAKE_THREAD_LIBS_INIT})

add_definitions("-Werror -Wall -Wno-unused-result")

if(${BUILD_DEPS} STREQUAL "Yes")
    add_subdirectory(${DEPS}/SDL2)
    add_subdirectory(${DEPS}/SDL2_ttf)
else()
    find_program(SDL_CONFIG sdl2-config)
    execute_process(COMMAND "${SDL_CONFIG}" "--libs"
                    OUTPUT_VARIABLE sdl_libs
                    OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND ${SDL_CONFIG} "--cflags"
                    OUTPUT_VARIABLE sdl_cflags
                    OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()

target_link_libraries(gbemu SDL2 SDL2_ttf m)
