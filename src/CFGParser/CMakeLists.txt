set(CFG_PARSER_SOURCES
    CFGParser.cpp
)

add_library(CFGParser STATIC ${CFG_PARSER_SOURCES})
target_include_directories(CFGParser PRIVATE ${PROJECT_SOURCE_DIR}/src)