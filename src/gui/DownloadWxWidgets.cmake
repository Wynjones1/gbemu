if(MSVC)
	if(${CMAKE_SIZEOF_VOID_P} STREQUAL "8")
		set(PREFIX "_x64")
	else()
		set(PREFIX "")
	endif()
	set(DevDLL  https://github.com/wxWidgets/wxWidgets/releases/download/v3.1.0/wxMSW-3.1.0_vc140${PREFIX}_Dev.7z)
	set(RelDLL  https://github.com/wxWidgets/wxWidgets/releases/download/v3.1.0/wxMSW-3.1.0_vc140${PREFIX}_ReleaseDLL.7z)
	set(Headers https://github.com/wxWidgets/wxWidgets/releases/download/v3.1.0/wxWidgets-3.1.0-headers.7z)

	set(OUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/wxWidgetsDownload)
	make_directory(${OUT_DIR})

	foreach(URL DevDLL RelDLL Headers)
		set(OUT_FILE ${OUT_DIR}/${URL}.7z)
		if(NOT EXISTS ${OUT_DIR}/${URL}.7z)
			message(STATUS "Downloading ${${URL}}")
			file(DOWNLOAD ${${URL}} ${OUT_FILE})
		endif()
		if(NOT EXISTS ${OUT_DIR}/${URL}.stamp)
			message(STATUS "Extracting ${OUT_FILE}")
			execute_process(
				COMMAND ${CMAKE_COMMAND} -E tar xvf ${OUT_FILE}
				COMMAND ${CMAKE_COMMAND} -E touch ${OUT_DIR}/${URL}.stamp
				WORKING_DIRECTORY ${OUT_DIR}
			)
		endif()
	endforeach()

	file(GLOB_RECURSE WX_DLLS ${OUT_DIR}/lib/vc140${PREFIX}_dll/*.dll)

	set(WX_LINK_DIRS    ${OUT_DIR}/lib/vc140${PREFIX}_dll)
	set(WX_INCLUDE_DIRS ${OUT_DIR}/include/msvc ${OUT_DIR}/include)
	set(WX_DEFINITIONS -DwxMSVC_VERSION_AUTO -DWXUSINGDLL -D_CRT_SECURE_NO_WARNINGS -D_UNICODE)
else()
	message(FATAL_ERROR "Please Fill In")
endif()