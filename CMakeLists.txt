cmake_minimum_required(VERSION 2.8)

list(APPEND CMAKE_MODULE_PATH
            ${CMAKE_SOURCE_DIR}/cmake
            ${CMAKE_SOURCE_DIR}/deps
			${CMAKE_SOURCE_DIR}/src
)
if(MINGW)
    include(mingw-toolchain)
endif()

project(gbemu)
find_package(SDL2)
find_package(SDL2_ttf)

set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type")

include(macros)
include(coverage)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
	set(CMAKE_C_FLAGS_RELEASE -Os)
	set(CMAKE_C_FLAGS --std=c99)
endif()

set(SDL_SHARED No)

#################################################################
#                          Definitions 
#################################################################
if(MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

#################################################################
#                            Options 
#################################################################
DEFADD(AUDIO               Yes  "Build with audio")
DEFADD(OUTPUT_WARNINGS     Yes  "Enable warning output.")
DEFADD(OUTPUT_ERRORS       Yes  "Enable error output.")
DEFADD(OUTPUT_OUTPUT       No   "Enable stdout  output.")
DEFADD(REGISTER_WINDOW     No   "Enable GB register window")
DEFADD(OUTPUT_DEBUG_FILES  No   "Output debug output files")
DEFADD(DEBUG_WINDOW        No   "Window with current instruction and register values")
DEFADD(SPINLOCK            No   "Use busy wait to framelimit.")
DEFADD(INTRO_ONLY          No   "Only run the nintendo intro.")
DEFADD(CONTROLS            No   "Display GameBoy controls")
DEFADD(LOG_INSTRUCTIONS    No   "Log each instruction executed")
DEFADD(HAVE_TTF            Yes  "Have TTF Support")

set(COVERAGE    No                        CACHE BOOL    "Generate coverage data")
set(CHECK       No                        CACHE BOOL    "add -weverything option")
set(DEPS_DIR    ${CMAKE_SOURCE_DIR}/deps  CACHE INTERNAL "")
set(UTILS_DIR   ${CMAKE_SOURCE_DIR}/utils CACHE INTERNAL "")
if(MSVC)
	set(SDL_DEBUG_LIBS No CACHE BOOL "Set if the debug builds of SDL2 should be used.")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#Use these options to include the boot_rom + rom in the executable
DEFADD(EMBEDDED_FILES      No "Embbed all files in binary")
if(EMBEDDED_FILES)
	set(EMBEDDED_BOOT_ROM_FILE ${CMAKE_SOURCE_DIR}/data/boot_roms/DMG.bin CACHE FILEPATH "Initial 256 byte program to be embedded in the executable")
	set(EMBEDDED_ROM_FILE ${CMAKE_SOURCE_DIR}/data/roms/mario.gb          CACHE FILEPATH "Game to be embedded in the executable")
endif()

find_library(SDL2_LIB     SDL2)
find_library(SDL2_TTF_LIB SDL2_ttf)

#TODO: For now this needs to be before the src dir because we define stuff it needs. Add a FindPackage at some point.
if(EMBEDDED_FILES)
	add_subdirectory(${UTILS_DIR}/create_header)
else()
	set(EMBEDDED_GENERATED_FILES)
endif()

add_subdirectory(src)
add_subdirectory(tests)

if(MINGW)
    set(DIRECTX No)
    add_definitions(-mwindows)
endif()

function(add_target_folder TARGET_NAME DIR)
	if(TARGET ${TARGET_NAME})
		set_target_properties(${TARGET_NAME} PROPERTIES FOLDER ${DIR})
	endif()
endfunction()

if(MSVC)
	set(DIRECTX No)
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)
	add_target_folder(SDL2           "Dependencies//SDL2")
	add_target_folder(SDL_ttf        "Dependencies//SDL2")
	add_target_folder(SDL2-static    "Dependencies//SDL2")
	add_target_folder(SDL2main       "Dependencies//SDL2")
	add_target_folder(freetype       "Dependencies//Freetype")
	add_target_folder(dist           "Dependencies//Freetype")
	add_target_folder(create_header  "Utils//Create Headers")
	add_target_folder(create_headers "Utils//Create Headers")
	add_target_folder(embedded_files "Dependencies//Generated Files")
endif()
