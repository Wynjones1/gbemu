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
		-Wno-unused-variable
		-Wno-enum-conversion
		-Wno-unused-parameter
		-Wno-unused-function
	)
    if(CMAKE_C_COMPILER_ID MATCHES "Clang")
        add_definitions(
            -Wno-typedef-redefinition
            -Wno-unused-const-variable
        )
    else()
        add_definitions(
        	-Wno-unused-result
            -Wno-unused-but-set-variable
        )
    endif()

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

set(ALL_SRC    ${SRC}    ${SRC_INSTRUCTIONS}    ${SRC_PREFIX_CB})
set(ALL_HEADER ${HEADER} ${HEADER_INSTRUCTIONS} ${HEADER_PREFIX_CB})

#Remove the entry point from the library. it will be built in the executable
set(LIB_SRC ${ALL_SRC})
list(REMOVE_ITEM LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/main.c)

add_library(gbemu_lib ${LIB_SRC} ${ALL_HEADER})

if(EMBEDDED_FILES)
    target_link_libraries(gbemu_lib embedded_files)
endif()

if(BUILD_SDL)
	if(MSVC)
		target_link_libraries(gbemu_lib SDL2main SDL2-static)
	else()
		target_link_libraries(gbemu_lib SDL2-static)
	endif()
else()
    target_link_libraries(gbemu_lib SDL2)
endif()

if(BUILD_TTF)
	if(NOT MSVC)
		target_link_libraries(gbemu_lib m)
	endif()
    target_link_libraries(gbemu_lib SDL2_ttf freetype)
else()
    target_link_libraries(gbemu_lib SDL2_ttf)
endif()

add_executable(gbemu main.c)
target_link_libraries(gbemu gbemu_lib)
