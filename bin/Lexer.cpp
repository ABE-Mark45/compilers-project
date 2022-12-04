#include <vector>
#include "DFABuilder/DFABuilder.h"
#include "LanguageRulesParser/LanguageRulesParser.h"
#include "NFABuilder/NFABuilder.h"
#include "Readers/LanguageRulesReader.h"
#include "Readers/ProgramReader.h"
#include "Simulator/Simulator.h"

auto main(int argc, char** argv) -> int {
  if (argc != 3) {
    throw std::runtime_error("Lexer <rules_path> <program_path>");
  }
  const std::filesystem::path rulesFilePath{argv[1]};
  LanguageRulesReader rulesReader(rulesFilePath);

  const std::filesystem::path programFilePath{argv[2]};
  ProgramReader programReader(programFilePath);

  LanguageRulesParser parser;
  while (auto rule = rulesReader.getLine()) {
    parser.parseLine(rule.value());
  }

  nfa::NFABuilder nfaBuilder(
      parser.getPostfixRegexExpressions(), parser.getKeywords(),
      parser.getPunctuationCharacters(), parser.getPriorities());

  auto nfa = nfaBuilder.getCombinedNFA();

  auto dfaStartState = DFABuilder::buildDFA(std::move(nfa));

  Simulator s(dfaStartState);

  while (programReader.hasChar()) {
    s.consumeCharacter(programReader.getChar());
  }
  return 0;
}