set(SDL2_PREFIX       ${CMAKE_CURRENT_SOURCE_DIR}/deps/SDL2)
set(SDL2_FOUND        True)
set(SDL2_INCLUDE_DIRS "")
set(SDL2_LIBRARIES    "")
set(SDL2_LIBRARY_PATH "")
set(SDL2_FOUND        False)

if(MSVC)
    set(SDL2_FOUND True)
	set(SDL2_INCLUDE_DIRS ${SDL2_PREFIX}/Win32/include)

	# Check if we are building 64 bits.
	if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
		set(SDL2_LIBRARY_PATH ${SDL2_PREFIX}/Win32/lib/x64)
	else()
		set(SDL2_LIBRARY_PATH ${SDL2_PREFIX}/Win32/lib/x86)
	endif()
	
	if(SDL_DEBUG_LIBS)
		set(SDL2_LIBRARY_PATH ${SDL2_LIBRARY_PATH}_debug)
	endif()
	
	set(SDL2_LIBRARIES
		${SDL2_LIBRARY_PATH}/SDL2.lib
		${SDL2_LIBRARY_PATH}/SDL2main.lib
	)
else()
    include(FindPkgConfig)
    pkg_check_modules(SDL2 sdl2)
    if(NOT SDL2_FOUND)
        message(STATUS "Building SDL2 from source...")
        include(ExternalProject)
        set(INSTALL_DIR ${CMAKE_BINARY_DIR}/external/)
        ExternalProject_Add(
            SDL2
             URL https://www.libsdl.org/release/SDL2-2.0.4.tar.gz
             CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR}
        )
        set( ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:${INSTALL_DIR}/lib/pkgconfig")
        pkg_check_modules(SDL2 sdl2 QUIET)
        set(SDL2_LIBRARIES ${SDL2_LIBRARY_PATH}/libSDL2.a ${SDL2_LIBRARY_PATH}/libSDL2main.a)
    endif()
endif()
