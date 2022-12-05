add_executable(DFABuilderTest DFABuilderTest.cpp)

target_link_libraries(DFABuilderTest
PUBLIC
GTest::GTest
NFA
DFA
DFABuilder
LanguageRulesParser
NFABuilder
Readers
Simulator)