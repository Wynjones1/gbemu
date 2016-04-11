cmake_minimum_required(VERSION 2.8)

set(TEST_LIST
	"cgb_sound"
	"cpu_instrs"
	"dmg_sound-2"
	"instr_timing"
	"mem_timing-2"
	"oam_bug-2"
)

find_package(GBEmu)

include_directories(${GBEMU_INCLUDE_DIRS})

foreach(NAME ${TEST_LIST})
	set(URL http://slack.net/~ant/old/gb-tests/${NAME}.zip)
	set(FILE ${CMAKE_CURRENT_BINARY_DIR}/${NAME}.zip)
	if(NOT EXISTS ${FILE})
		file(DOWNLOAD ${URL} ${FILE})
		execute_process(
			COMMAND ${CMAKE_COMMAND} -E tar xvf ${FILE} -- ${CMAKE_CURRENT_BINARY_DIR}/${NAME}
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		)
	endif()

	set(OUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/${NAME})
	file(GLOB ROMS ${OUT_DIR}/*.gb)

	add_executable(${NAME} main.c ${ROMS})
	target_link_libraries(${NAME} ${GBEMU_LIBRARIES})
	target_compile_definitions(${NAME} PUBLIC -DROM_FILE="${ROMS}")
	set_property(TARGET ${NAME} PROPERTY FOLDER "Tests//${NAME}")
endforeach()