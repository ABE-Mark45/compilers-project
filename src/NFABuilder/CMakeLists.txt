set(NFA_BUILDER_SOURCES
    NFABuilder.cpp
)

add_library(NFA_BUILDER STATIC ${NFA_BUILDER_SOURCES})
target_include_directories(NFA_BUILDER PRIVATE ${PROJECT_SOURCE_DIR}/src)