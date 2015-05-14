cmake_minimum_required(VERSION 2.8)

list(APPEND CMAKE_MODULE_PATH
            ${CMAKE_SOURCE_DIR}/cmake
            ${CMAKE_SOURCE_DIR}/deps)
if(MINGW)
    include(mingw-toolchain)
endif()

project(gbemu)
find_package(SDL2)
find_package(TTF)
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
set(COVERAGE               No    CACHE BOOL   "Generate coverage data")
set(CHECK                  No    CACHE BOOL   "add everything option")
DEFADD(AUDIO               Yes  "Build with audio")
DEFADD(OUTPUT_WARNINGS     Yes  "Enable warning output.")
DEFADD(OUTPUT_ERRORS       Yes  "Enable error output.")
DEFADD(OUTPUT_OUTPUT       No   "Enable stdout  output.")
DEFADD(TESTING             No   "Run tests.")
DEFADD(REGISTER_WINDOW     No   "Enable GB register window")
DEFADD(OUTPUT_DEBUG_FILES  No   "Output debug output files")
DEFADD(DEBUG_WINDOW        No   "Window with current instruction and register values")
DEFADD(SPINLOCK            No   "Use busy wait to framelimit.")
DEFADD(INTRO_ONLY          No   "Only run the nintendo intro.")
DEFADD(CONTROLS            No   "Display GameBoy controls")
DEFADD(LOG_INSTRUCTIONS    No   "Log each instruction executed")
DEFADD(BUILD_SDL           No   "Build SDL2 from source.")
DEFADD(BUILD_TTF           No   "Build TTF  from source.")
DEFADD(HAVE_TTF            Yes  "Have TTF Support")

#Use these options to include the boot_rom + rom in the executable
DEFADD(EMBEDDED_FILES      Yes  "Embbed all files in binary")
if(EMBEDDED_FILES)
	set(EMBEDDED_BOOT_ROM_FILE ${CMAKE_SOURCE_DIR}/data/boot_roms/DMG.bin CACHE FILEPATH "Initial 256 byte program to be embedded in the executable")
	set(EMBEDDED_ROM_FILE ${CMAKE_SOURCE_DIR}/data/roms/mario.gb          CACHE FILEPATH "Game to be embedded in the executable")
endif()


set(DEPS_DIR  ${CMAKE_SOURCE_DIR}/deps CACHE INTERNAL "")
set(UTILS_DIR ${CMAKE_SOURCE_DIR}/utils CACHE INTERNAL "")

#TODO: For now this needs to be before the src dir because we define stuff it needs. Add a FindPackage at some point.
if(EMBEDDED_FILES)
	add_subdirectory(${UTILS_DIR}/create_header)
else()
	set(EMBEDDED_GENERATED_FILES)
endif()

add_subdirectory(src)

if(TESTING)
	add_subdirectory(tests)
endif()


if(MINGW)
    set(DIRECTX No)
    add_definitions(-mwindows)
endif()

if(MSVC)
	set(DIRECTX No)
endif()

if(BUILD_SDL OR MINGW)
    add_subdirectory(${DEPS_DIR}/SDL2)
endif()

if(BUILD_TTF OR MINGW)
    add_subdirectory(${DEPS_DIR}/freetype-2.5.5)
    add_subdirectory(${DEPS_DIR}/SDL2_ttf)
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
	add_target_folder(freetype       "Dependencies//freetype")
	add_target_folder(dist           "Dependencies//freetype")
	add_target_folder(create_header  "Utils//Create Headers")
	add_target_folder(create_headers "Utils//Create Headers")
endif()
