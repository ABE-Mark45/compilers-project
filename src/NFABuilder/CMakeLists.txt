set(NFA_BUILDER_SOURCES
    NFABuilder.cpp
)

add_library(NFABuilder STATIC ${NFA_BUILDER_SOURCES})
target_include_directories(NFABuilder PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(NFABuilder PUBLIC NFA)