if(COVERAGE)
    add_definitions("--coverage")
    link_libraries(gcov)
endif()
