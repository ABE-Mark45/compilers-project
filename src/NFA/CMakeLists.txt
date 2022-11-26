set(NFA_SOURCES
    State.cpp
    NFA.cpp
)

add_library(NFA STATIC ${NFA_SOURCES})
target_include_directories(NFA PRIVATE ${PROJECT_SOURCE_DIR}/src)