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
	# Add the toplevel rom
	file(GLOB ROM ${OUT_DIR}/*.gb)

	add_executable(${NAME} main.c ${ROM})
	target_link_libraries(${NAME} ${GBEMU_LIBRARIES})
		target_compile_definitions(${NAME} PUBLIC -DROM_FILE="${ROM}")
	set_property(TARGET ${NAME} PROPERTY FOLDER "Tests//${NAME}")

	foreach(SUB "rom_singles" "individual")
		set(DIR ${OUT_DIR}/${SUB})
		file(GLOB ROMS RELATIVE  ${DIR} ${DIR}/*.gb)

		foreach(ROM ${ROMS})
			set(TARGET_NAME ${NAME}_${ROM})
			foreach(X " " "," "-" "(" ")")
				string(REPLACE ${X} "_" TARGET_NAME ${TARGET_NAME})
			endforeach()

			add_executable(${TARGET_NAME} main.c ${DIR}/${ROM})
			set_property(TARGET ${TARGET_NAME} PROPERTY PROJECT_LABEL ${ROM})
			target_link_libraries(${TARGET_NAME} ${GBEMU_LIBRARIES})
			target_compile_definitions(${TARGET_NAME} PUBLIC -DROM_FILE="${DIR}/${ROM}")
			set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER "Tests//${NAME}//Sub Tests")
		endforeach()
	endforeach()
endforeach()