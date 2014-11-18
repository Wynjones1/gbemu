cmake_minimum_required(VERSION 2.8)
project(gbemu)

set(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(DEPS ${ROOT}/deps)

file(GLOB SRC
	"${ROOT}/src/*.cpp"
	"${ROOT}/src/instructions/*.cpp"
	"${ROOT}/src/instructions/prefix_cb/*.cpp"
	"${ROOT}/src/testing/*.cpp")

file(GLOB HEADERS
	"${ROOT}/src/*.h"
	"${ROOT}/src/instructions/*.h"
	"${ROOT}/src/instructions/prefix_cb/*.h"
	"${ROOT}/src/testing/*.h")

include_directories("./src" "./src/back_end" "./src/front_end")

set(CMAKE_BUILD_TYPE "Debug" CACHE BOOL "Build type")
set(BUILD_DEPS       "No"    CACHE BOOL "Build dependancies from source")
set(RUN_TESTS        "No"    CACHE BOOL "Run Test Suite")

add_executable(gbemu ${SRC})

macro(DEFADD NAME DEFAULT DESC)
	set(${NAME} ${DEFAULT} CACHE BOOL ${DESC})
	if(${NAME} STREQUAL "Yes")
		add_definitions(-D${NAME}=1)
	else()
		add_definitions(-D${NAME}=0)
	endif()
endmacro()

macro(ADD_CFLAG arg)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${arg}")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${arg}")
endmacro()

DEFADD(OUTPUT_WARNINGS "No"  "Enable warning output.")
DEFADD(OUTPUT_ERRORS   "No"  "Enable warning output.")
DEFADD(OUTPUT_OUTPUT   "No"  "Enable stdout  output.")
DEFADD(DISPLAY_ENABLED "Yes" "Use display.")
DEFADD(DISPLAY_THREAD  "Yes" "Display drawing on separate thread.")
DEFADD(TESTING         "No"  "Run tests.")
DEFADD(AUDIO_ENABLED   "No"  "Enable Audio")
ADD_CFLAG("-m32")

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	add_definitions(-DDEBUG=1)
else()
	add_definitions(-DDEBUG=0)
endif()

include_directories(${DEPS}/SDL2/include)

if(MSVC)
	add_subdirectory(${DEPS}/SDL2)
	add_subdirectory(${DEPS}/SDL2_ttf)
else()
	if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
		ADD_CFLAG("-Werror -Wall")
	endif()

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
endif()

target_link_libraries(gbemu pthread SDL2 SDL2_ttf)

