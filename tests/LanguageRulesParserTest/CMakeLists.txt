add_executable(LanguageRulesParserTest LanguageRulesParserTest.cpp)

target_link_libraries(LanguageRulesParserTest
PUBLIC
GTest::GTest
LanguageRulesParser)