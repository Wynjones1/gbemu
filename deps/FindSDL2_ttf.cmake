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
endif()