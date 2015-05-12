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
        -Wno-unused-but-set-variable
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

source_group("Headers"                           FILES ${HEADER})
source_group("Headers\\Instructions"             FILES ${HEADER_INSTRUCTIONS})
source_group("Headers\\Instructions\\Prefix CB"  FILES ${HEADER_PREFIX_CB})
source_group("Sources"                           FILES ${SRC})
source_group("Sources\\Instructions"             FILES ${SRC_INSTRUCTIONS})
source_group("Sources\\Instructions\\Prefix CB"  FILES ${SRC_PREFIX_CB})
source_group("Sources\\Generated"  FILES ${EMBEDDED_GENERATED_FILES})

set(ALL_SRC    ${SRC}    ${SRC_INSTRUCTIONS}    ${SRC_PREFIX_CB}    ${EMBEDDED_GENERATED_FILES})
set(ALL_HEADER ${HEADER} ${HEADER_INSTRUCTIONS} ${HEADER_PREFIX_CB})

set_source_files_properties(${EMBEDDED_GENERATED_FILES} PROPERTIES GENERATED TRUE)

#Remove the entry point from the library. it will be built in the executable
set(LIB_SRC ${ALL_SRC})
list(REMOVE_ITEM LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/main.c)

add_library(gbemu_lib ${LIB_SRC} ${ALL_HEADER})
add_executable(gbemu main.c)

if(MINGW)
    target_link_libraries(gbemu mingw32 SDL2main SDL2-static)
elseif(MSVC)
	target_link_libraries(gbemu_lib SDL_ttf SDL2main SDL2-static freetype)
else()
    target_link_libraries(gbemu_lib SDL2 SDL_ttf)
endif()

target_link_libraries(gbemu gbemu_lib)
