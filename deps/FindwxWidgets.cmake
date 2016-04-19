if(DEFINED WX_PREFIX)
	return()
endif()

get_property(WX_DOWNLOADED GLOBAL PROPERTY WX_DOWNLOADED SET)
if(NOT WX_DOWNLOADED)
	message(FATAL_ERROR "DownloadWxWidgets.cmake has not been called.")
endif()

get_property(WX_PREFIX      GLOBAL PROPERTY WX_PREFIX)
get_property(WX_ARCH_PREFIX GLOBAL PROPERTY WX_ARCH_PREFIX)

set(WX_DEFINITIONS -DwxMSVC_VERSION_AUTO -DWXUSINGDLL -D_CRT_SECURE_NO_WARNINGS -D_UNICODE)
set(WX_LINK_DIRS    ${WX_PREFIX}/lib/vc140${WX_ARCH_PREFIX}_dll)
set(WX_INCLUDE_DIRS ${WX_PREFIX}/include/msvc ${WX_PREFIX}/include)
set(WX_DLLS_DIR ${WX_PREFIX}/lib/vc140${WX_ARCH_PREFIX}_dll)
file(GLOB_RECURSE WX_DLLS ${WX_DLLS_DIR}/*.dll)