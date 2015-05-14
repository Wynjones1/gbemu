cmake_minimum_required(VERSION 2.8)
project(create_header)

add_executable(create_header create_header.c)
set(GENERATED_SOURCE_DIR     ${CMAKE_BINARY_DIR}/generated_source CACHE INTERNAL "")
set(BOOT_ROM_OUTPUT_FILE     ${GENERATED_SOURCE_DIR}/embedded_boot_rom.c)
set(ROM_OUTPUT_FILE          ${GENERATED_SOURCE_DIR}/embedded_rom.c)
set(EMBEDDED_GENERATED_FILES ${BOOT_ROM_OUTPUT_FILE} ${ROM_OUTPUT_FILE})


if(NOT EXISTS ${EMBEDDED_BOOT_ROM_FILE})
    message(FATAL_ERROR "Boot ROM file ${EMBEDDED_BOOT_ROM_FILE} does not exists.")
endif()

if(NOT EXISTS ${EMBEDDED_ROM_FILE})
    message(FATAL_ERROR "ROM file ${EMBEDDED_ROM_FILE} does not exists.")
endif()

add_custom_command(OUTPUT ${BOOT_ROM_OUTPUT_FILE} ${ROM_OUTPUT_FILE}
					COMMAND ${CMAKE_COMMAND} -E make_directory ${GENERATED_SOURCE_DIR} 
					COMMAND create_header ${EMBEDDED_BOOT_ROM_FILE} ${BOOT_ROM_OUTPUT_FILE} "boot"
					COMMAND create_header ${EMBEDDED_ROM_FILE}      ${ROM_OUTPUT_FILE}      "rom"
					DEPENDS create_header)
add_custom_target(create_headers DEPENDS ${BOOT_ROM_OUTPUT_FILE} ${ROM_OUTPUT_FILE})

set_source_files_properties(${EMBEDDED_GENERATED_FILES} PROPERTIES GENERATED TRUE)
add_library(embedded_files ${EMBEDDED_GENERATED_FILES})
