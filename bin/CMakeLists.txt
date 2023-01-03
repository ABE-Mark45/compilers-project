add_executable(lexer Lexer.cpp)

include_directories(${PROJECT_SOURCE_DIR}/src)
target_link_libraries(lexer
PUBLIC
NFA
DFA
DFABuilder
LanguageRulesParser
NFABuilder
Readers
Simulator
CFGParser
LL1Generator
ParseTableGenerator
)