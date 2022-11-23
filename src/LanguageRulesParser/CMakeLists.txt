set(LANGUAGE_RULES_PARSER_SOURCES
    LanguageRulesParser.cpp
)

add_library(LanguageRulesParser STATIC ${LANGUAGE_RULES_PARSER_SOURCES})
target_include_directories(LanguageRulesParser PRIVATE ${PROJECT_SOURCE_DIR}/src)