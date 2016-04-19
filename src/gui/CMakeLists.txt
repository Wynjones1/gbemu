cmake_minimum_required(VERSION 2.8)
find_package(wxWidgets REQUIRED)
find_package(OpenGL    REQUIRED)

link_directories(${WX_LINK_DIRS})
include_directories(
	${WX_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIR}
)

add_definitions(${WX_DEFINITIONS})
add_executable(wxGbemu WIN32 main.cpp)

add_library(platform_wx ${CMAKE_CURRENT_SOURCE_DIR}/platform_wx.cpp)

target_link_libraries(wxGbemu platform_wx gbemu_lib ${OPENGL_LIBRARIES})

add_custom_command(
	TARGET wxGbemu
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${WX_DLLS_DIR} $<TARGET_FILE_DIR:wxGbemu>
)