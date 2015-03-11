file(GLOB SRC	              "${ROOT}/src/*.cpp")
file(GLOB SRC_INSTRUCTIONS    "${ROOT}/src/instructions/*.cpp")
file(GLOB SRC_PREFIX_CB       "${ROOT}/src/instructions/prefix_cb/*.cpp")

file(GLOB HEADER	          "${ROOT}/src/*.h")
file(GLOB HEADER_INSTRUCTIONS "${ROOT}/src/instructions/*.h")
file(GLOB HEADER_PREFIX_CB    "${ROOT}/src/instructions/prefix_cb/*.h")

source_group(headers                          FILES ${HEADER})
source_group(headers\\instructions            FILES ${HEADER_INSTRUCTIONS})
source_group(headers\\instructions\\prefix_cb FILES ${HEADER_PREFIX_CB})

source_group(src                          FILES ${SRC})
source_group(src\\instructions            FILES ${SRC_INSTRUCTIONS})
source_group(src\\instructions\\prefix_cb FILES ${SRC_PREFIX_CB})

if(TESTING)
    file(GLOB SRC_TESTING         "${ROOT}/src/testing/*.cpp")
    file(GLOB HEADER_TESTING      "${ROOT}/src/testing/*.h")
    source_group(headers\\testing  FILES ${HEADER_TESTING})
    source_group(src\\testing      FILES ${SRC_TESTING})
endif()

include_directories("./src" "./src/back_end" "./src/front_end")

set(ALL_SRC    ${SRC}    ${SRC_INSTRUCTIONS}    ${SRC_PREFIX_CB}    ${SRC_TESTING})
set(ALL_HEADER ${HEADER} ${HEADER_INSTRUCTIONS} ${HEADER_PREFIX_CB} ${HEADER_TESTING})
