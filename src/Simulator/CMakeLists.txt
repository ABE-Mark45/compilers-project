set(SIMULATOR_SOURCES
    LexicalSimulator.cpp
    ParserSimulator.cpp
)

add_library(Simulator STATIC ${SIMULATOR_SOURCES})
target_include_directories(Simulator PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(Simulator PUBLIC DFA)