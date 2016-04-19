cmake_minimum_required(VERSION 2.8)

include(ExternalProject)

ExternalProject_Add(WLA_DX
	GIT_REPOSITORY https://github.com/vhelin/wla-dx
	INSTALL_COMMAND ""
)

add_executable(wla_gb  IMPORTED)
add_executable(wlalink IMPORTED)
set(BINARY_DIRECTORY ${CMAKE_BINARY_DIR}/deps/WLA_DX-prefix/src/WLA_DX-build/binaries/Debug)
set_property(TARGET wla_gb  PROPERTY IMPORTED_LOCATION ${BINARY_DIRECTORY}/wla-gb.exe)
set_property(TARGET wlalink PROPERTY IMPORTED_LOCATION ${BINARY_DIRECTORY}/wlalink.exe)

include(DownloadWxWidgets.cmake)