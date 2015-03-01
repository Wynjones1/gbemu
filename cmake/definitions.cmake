add_definitions(--std=c11)
add_definitions(-Werror -Wall -Wextra)
add_definitions(-Wno-enum-conversion)
add_definitions(-Wno-unused-function
                -Wno-unused-parameter
                -Wno-unused-result
                -Wno-unused-variable)
if(CHECK)
    add_definitions(-Weverything)
    add_definitions(-Wno-missing-prototypes)
    add_definitions(-Wno-padded)
    add_definitions(-Wno-gnu-zero-variadic-macro-arguments)
    add_definitions(-Wno-covered-switch-default)
    set(CMAKE_C_COMPILER clang)
endif()
