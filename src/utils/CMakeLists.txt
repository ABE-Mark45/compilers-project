add_library(utils INTERFACE)
target_include_directories(utils INTERFACE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(utils INTERFACE DFA)