set(SDL2_TTF_PREFIX       ${CMAKE_CURRENT_SOURCE_DIR}/deps/SDL2_ttf)
set(SDL2_TTF_FOUND        True)
set(SDL2_TTF_INCLUDE_DIRS "")
set(SDL2_TTF_LIBRARIES    "")
set(SDL2_TTF_LIBRARY_PATH "")

if(MSVC)
	set(SDL2_TTF_INCLUDE_DIRS ${SDL2_TTF_PREFIX}/Win32/include)
	
	# Check if we are building 64 bits.
	if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
		set(SDL2_TTF_LIBRARY_PATH ${SDL2_TTF_PREFIX}/Win32/lib/x64)
	else()
		set(SDL2_TTF_LIBRARY_PATH ${SDL2_TTF_PREFIX}/Win32/lib/x86)
	endif()
	
	set(SDL2_TTF_LIBRARIES
		${SDL2_TTF_LIBRARY_PATH}/SDL2_ttf.lib
	)
else()
    include(FindPkgConfig)
    # If we can't find the library then we will
    # build it from source, this is mainly because
    # TravisCI does not contain SDL2 in it's repos
    pkg_check_modules(SDL2_TTF QUIET SDL2_ttf)
    if(NOT "${SDL2_TTF_FOUND}" STREQUAL "1")
        message(STATUS "SDL2_ttf needs to be built from source...")
        # Download and build the library.
        include(ExternalProject)
        set(INSTALL_DIR ${CMAKE_BINARY_DIR}/external)
        ExternalProject_Add(
            SDL2_TTF_BUILD
            URL https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-2.0.14.tar.gz
            CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=${INSTALL_DIR}
        )
        # Setup the include directories.
        set(SDL2_TTF_INCLUDE_DIRS ${INSTALL_DIR}/include/SDL2)
        # Setup the library.
        add_library(SDL2_ttf IMPORTED SHARED)
        set_property(TARGET SDL2_ttf PROPERTY IMPORTED_LOCATION ${INSTALL_DIR}/lib/libSDL2_ttf.so)
        set(SDL2_TTF_LIBRARIES SDL2_ttf)
        # Make sure the library is built before we use it.
        add_dependencies(SDL2_ttf SDL2_TTF_BUILD)
        if(TARGET SDL2_BUILD)
            add_dependencies(SDL2_TTF_BUILD SDL2_BUILD)
        endif()
    endif()
endif()
