set(DFA_BUILDER_SOURCES
    DFABuilder.cpp
)

add_library(DFABuilder STATIC ${DFA_BUILDER_SOURCES})
target_include_directories(DFABuilder PRIVATE ${PROJECT_SOURCE_DIR}/src)