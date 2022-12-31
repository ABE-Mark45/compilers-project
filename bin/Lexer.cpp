#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "DFABuilder/DFABuilder.h"
#include "LanguageRulesParser/LanguageRulesParser.h"
#include "NFABuilder/NFABuilder.h"
#include "Readers/LanguageRulesReader.h"
#include "Readers/ProgramReader.h"
#include "Simulator/LexicalSimulator.h"
#include "utils/DFAPrinter.h"

auto main(int argc, char** argv) -> int {
  if (argc != 5) {
    throw std::runtime_error(
        "Lexer <rules_path> <program_path> <table_out_path> <tokens_out_path>");
  }
  const std::filesystem::path rulesFilePath{argv[1]};
  LanguageRulesReader rulesReader(rulesFilePath);

  const std::filesystem::path programFilePath{argv[2]};
  ProgramReader programReader(programFilePath);

  const std::filesystem::path tableOutputPath{argv[3]};
  std::ofstream tableOutputFile(tableOutputPath,
                                std::ios::out | std::ios::trunc);

  const std::filesystem::path tokensOutputPath{argv[4]};
  std::ofstream tokensOutputFile(tokensOutputPath,
                                 std::ios::out | std::ios::trunc);

  LanguageRulesParser parser;
  while (auto rule = rulesReader.getLine()) {
    parser.parseLine(rule.value());
  }

  nfa::NFABuilder nfaBuilder(
      parser.getPostfixRegexExpressions(), parser.getKeywords(),
      parser.getPunctuationCharacters(), parser.getPriorities());

  auto nfa = nfaBuilder.getCombinedNFA();

  
  auto dfaStartState =  DFABuilder::minimizeDFA(DFABuilder::buildDFA(std::move(nfa)));

  // lexical analysis
  LexicalSimulator s(dfaStartState);
  while (programReader.hasChar()) {
    s.consumeCharacter(programReader.getChar());
  }
  s.finishSimulation();

  // lexical analysis errors
  if(s.getErrors().size()) {
    std::cout << "found the following errors while doing lexical analysis:\n";
    for(auto& e: s.getErrors())
      std::cout << e << "\n";
  }

  vector<string> lexicalTokens = s.getTokens();


  // printDFA(dfaStartState, tableOutputFile);

  // ParserSimulator parserSim = ParserSimulator(table, topNT);

  // for(auto& token: lexicalTokens) {
  //   parserSim.consumeToken(token); // prints stack trace
  // }

  return 0;
}