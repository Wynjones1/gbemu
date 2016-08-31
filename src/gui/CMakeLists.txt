cmake_minimum_required(VERSION 2.8)


find_package(OpenGL REQUIRED)

foreach(CONFIG RELEASE DEBUG RELWITHDEBINFO)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONFIG} ${CMAKE_BINARY_DIR}/lib)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONFIG} ${CMAKE_BINARY_DIR}/lib)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG} ${CMAKE_BINARY_DIR}/bin)
endforeach()


if(MSVC)
	include(DownloadWxWidgets.cmake)
	include_directories(${WX_INCLUDE_DIRS})
	add_definitions(${WX_DEFINITIONS})
	link_directories(${WX_LINK_DIRS})
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")
	find_package(wxWindows COMPONENTS gl core base REQUIRED)
	include(${wxWidgets_USE_FILE})
endif()

add_executable(gui WIN32 main.cpp)

if(MSVC)
else()
	target_link_libraries(gui ${wxWidgets_LIBRARIES})
endif()


target_link_libraries(gui ${OPENGL_LIBRARIES})

foreach(FILE ${WX_DLLS})
	execute_process(
		COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/bin/
		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${FILE} ${CMAKE_BINARY_DIR}/bin/
	)
endforeach()





add_custom_target(run COMMAND gui)
