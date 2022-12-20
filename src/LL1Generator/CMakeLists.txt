set(LL1_GENERATOR_SOURCES
    LL1Generator.cpp
)

add_library(LL1Generator STATIC ${LL1_GENERATOR_SOURCES})
target_include_directories(LL1Generator PRIVATE ${PROJECT_SOURCE_DIR}/src)