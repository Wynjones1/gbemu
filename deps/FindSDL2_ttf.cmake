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
    #set(SDL2_TTF_LIBRARIES SDL2_ttf)
    pkg_check_modules(SDL2_TTF SDL2_ttf)
    if(NOT SDL2_TTF_FOUND)
        message(STATUS "Building SDL2_ttf from source...")
        include(ExternalProject)
        set(INSTALL_DIR ${CMAKE_BINARY_DIR}/external)
        ExternalProject_Add(
            SDL2_TTF
            URL https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-2.0.14.tar.gz
            CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=${INSTALL_DIR}
        )
        set(ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:${INSTALL_DIR}/lib/pkgconfig")
        pkg_check_modules(SDL2_TTF SDL2_ttf REQUIRED)
    endif()
endif()
