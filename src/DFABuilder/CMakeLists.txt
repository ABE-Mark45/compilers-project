set(DFA_BUILDER_SOURCES
    DFABuilder.cpp
)

add_library(DFA_BUILDER STATIC ${DFA_BUILDER_SOURCES})
target_include_directories(DFA_BUILDER PRIVATE ${PROJECT_SOURCE_DIR}/src)