set(READERS_SOURCES
    LanguageRulesReader.cpp
    ProgramReader.cpp
    CFGReader.cpp
)

add_library(Readers STATIC ${READERS_SOURCES})
target_include_directories(Readers PRIVATE ${PROJECT_SOURCE_DIR}/src)