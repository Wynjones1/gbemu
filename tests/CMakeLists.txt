cmake_minimum_required(VERSION 2.8)
project(gbemu_tests)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

include_directories(
	${CMAKE_SOURCE_DIR}/src/
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
)

add_executable(gbemu_tests ${SOURCES} ${HEADERS})
target_link_libraries(gbemu_tests gbemu_lib)