add_executable(CFGParserTest CFGParserTest.cpp)

target_link_libraries(CFGParserTest
PUBLIC
GTest::GTest
CFGParser)