#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "DFABuilder/DFABuilder.h"
#include "LanguageRulesParser/LanguageRulesParser.h"
#include "NFABuilder/NFABuilder.h"
#include "Readers/LanguageRulesReader.h"
#include "Readers/ProgramReader.h"
#include "Simulator/Simulator.h"
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

  auto dfaStartState =  DFABuilder::buildDFA(std::move(nfa));

  Simulator s(dfaStartState, tokensOutputFile);

  while (programReader.hasChar()) {
    s.consumeCharacter(programReader.getChar());
  }
  
  s.finishSimulation();

  printDFA(dfaStartState, tableOutputFile);
  return 0;
}