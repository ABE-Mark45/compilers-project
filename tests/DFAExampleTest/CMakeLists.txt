add_executable(DFAExampleTest DFAExampleTest.cpp)

target_link_libraries(DFAExampleTest
PUBLIC
GTest::GTest
LanguageRulesParser
NFA
NFABuilder
DFA
DFABuilder
Simulator
Readers)