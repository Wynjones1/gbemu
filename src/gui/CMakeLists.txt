cmake_minimum_required(VERSION 2.8)

find_package(wxWidgets REQUIRED gl core base)
find_package(OpenGL REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")

include_directories(${wxWidgets_INCLUDE_DIRS})

include(${wxWidgets_USE_FILE})
add_executable(gui main.cpp)
target_link_libraries(gui ${wxWidgets_LIBRARIES} ${OPENGL_LIBRARIES})

add_custom_target(run
    COMMAND gui
)
