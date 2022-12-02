set(DFA_SOURCES
    State.cpp
    DFA.cpp
)

add_library(DFA STATIC ${DFA_SOURCES})
target_include_directories(DFA PRIVATE ${PROJECT_SOURCE_DIR}/src)