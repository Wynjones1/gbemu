cmake_minimum_required(VERSION 2.8)

find_package(GBEmu)

include_directories(${GBEMU_INCLUDE_DIRS})

file(GLOB ROMS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*.gb)
foreach(ROM ${ROMS})
	set(TARGET_NAME ${ROM})
	foreach(X " " "," "-" "(" ")" "&")
		string(REPLACE ${X} "_" TARGET_NAME ${TARGET_NAME})
	endforeach()
	add_executable(${TARGET_NAME} main.c ${CMAKE_CURRENT_SOURCE_DIR}/${ROM})
	set_property(TARGET ${TARGET_NAME} PROPERTY PROJECT_LABEL ${ROM})
	target_link_libraries(${TARGET_NAME} ${GBEMU_LIBRARIES})
	target_compile_definitions(${TARGET_NAME} PUBLIC -DROM_FILE="${CMAKE_CURRENT_SOURCE_DIR}/${ROM}")
	set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER "Roms")
endforeach()