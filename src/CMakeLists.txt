cmake_minimum_required(VERSION 2.8)
project(gbemu)

#################################################################
#                          Definitions 
#################################################################

if(MSVC)
else()
	add_definitions(
		-Werror
		-Wall
		-Wextra
		-Wno-enum-conversion
		-Wno-unused-function
		-Wno-unused-parameter
		-Wno-unused-result
		-Wno-unused-variable
	)

	if(CHECK)
		add_definitions(
			-Weverything
			-Wno-missing-prototypes
			-Wno-padded
			-Wno-gnu-zero-variadic-macro-arguments
			-Wno-covered-switch-default
		)
		set(CMAKE_C_COMPILER clang)
	endif()
endif()

#################################################################
#                      Include Directories 
#################################################################
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/back_end
    ${CMAKE_SOURCE_DIR}/src/front_end
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
)

#################################################################
#                           Add Source
#################################################################
file(GLOB SRC	              "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
file(GLOB SRC_INSTRUCTIONS    "${CMAKE_CURRENT_SOURCE_DIR}/instructions/*.c")
file(GLOB SRC_PREFIX_CB       "${CMAKE_CURRENT_SOURCE_DIR}/instructions/prefix_cb/*.c")
file(GLOB HEADER	          "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB HEADER_INSTRUCTIONS "${CMAKE_CURRENT_SOURCE_DIR}/instructions/*.h")
file(GLOB HEADER_PREFIX_CB    "${CMAKE_CURRENT_SOURCE_DIR}/instructions/prefix_cb/*.h")

source_group(headers                          FILES ${HEADER})
source_group(headers\\instructions            FILES ${HEADER_INSTRUCTIONS})
source_group(headers\\instructions\\prefix_cb FILES ${HEADER_PREFIX_CB})
source_group(src                              FILES ${SRC})
source_group(src\\instructions                FILES ${SRC_INSTRUCTIONS})
source_group(src\\instructions\\prefix_cb     FILES ${SRC_PREFIX_CB})

if(TESTING)
    file(GLOB SRC_TESTING         "${CMAKE_CURRENT_SOURCE_DIR}testing/*.c")
    file(GLOB HEADER_TESTING      "${CMAKE_CURRENT_SOURCE_DIR}testing/*.h")
    source_group(headers\\testing  FILES ${HEADER_TESTING})
    source_group(src\\testing      FILES ${SRC_TESTING})
endif()


set(ALL_SRC    ${SRC}    ${SRC_INSTRUCTIONS}    ${SRC_PREFIX_CB}    ${SRC_TESTING})
set(ALL_HEADER ${HEADER} ${HEADER_INSTRUCTIONS} ${HEADER_PREFIX_CB} ${HEADER_TESTING})

add_executable(gbemu ${ALL_SRC} ${ALL_HEADER})

if(MINGW)
    target_link_libraries(gbemu mingw32 SDL2main SDL2-static)
elseif(MSVC)
	target_link_libraries(gbemu SDL_ttf SDL2main SDL2-static freetype)
else()
    target_link_libraries(gbemu SDL2 SDL_ttf)
endif()
