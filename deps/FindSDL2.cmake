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
    # If we can't find the library then we will
    # build it from source, this is mainly because
    # TravisCI does not contain SDL2 in it's repos
    pkg_check_modules(SDL2 QUIET sdl2)
    if(NOT SDL2_FOUND)
        message(STATUS "SDL2 needs to be built from source...")
        # Download and build the libraries.
        include(ExternalProject)
        set(INSTALL_DIR ${CMAKE_BINARY_DIR}/external/)
        ExternalProject_Add(
            SDL2_BUILD
            URL https://www.libsdl.org/release/SDL2-2.0.4.tar.gz
            CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR}
        )
        # Import the generated libraries into the build system.
        add_library(SDL2     IMPORTED SHARED)
        add_library(SDL2main IMPORTED STATIC)
        set_property(TARGET SDL2     PROPERTY IMPORTED_LOCATION ${INSTALL_DIR}/lib/libSDL2.so)
        set_property(TARGET SDL2main PROPERTY IMPORTED_LOCATION ${INSTALL_DIR}/lib/libSDL2main.a)
        set(SDL2_LIBRARIES SDL2 SDL2main)

        # Make sure the libraries are built before we use them.
        add_dependencies(SDL2     SDL2_BUILD)
        add_dependencies(SDL2main SDL2_BUILD)
    endif()
endif()
