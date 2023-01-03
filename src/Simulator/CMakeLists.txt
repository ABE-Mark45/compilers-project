set(SIMULATOR_SOURCES
    LexicalSimulator.cpp
    ParserSimulator.cpp
)

add_library(Simulator STATIC ${SIMULATOR_SOURCES})
target_include_directories(Simulator PUBLIC ${PROJECT_SOURCE_DIR}/src)