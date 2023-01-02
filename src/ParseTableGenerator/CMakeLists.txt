set(PARSE_TABLE_GENERATOR_SOURCES
    ParseTableGenerator.cpp
)

add_library(ParseTableGenerator STATIC ${PARSE_TABLE_GENERATOR_SOURCES})
target_include_directories(ParseTableGenerator PRIVATE ${PROJECT_SOURCE_DIR}/src)