cmake_minimum_required(VERSION 2.8)
project(gbemu)

file(GLOB SRC "./src/*.c" "./src/*.h")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/")

if(MSVC)
else()
	set(CMAKE_C_FLAGS "-std=c11")
endif()

add_executable(gbemu ${SRC})
