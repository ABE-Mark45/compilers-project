#include <gtest/gtest.h>
#include <vector>
#include "DFABuilder/DFABuilder.h"
#include "LanguageRulesParser/LanguageRulesParser.h"
#include "NFABuilder/NFABuilder.h"
#include "Readers/LanguageRulesReader.h"
#include "Readers/ProgramReader.h"
#include "Simulator/Simulator.h"

TEST(DFAExampleTest, test) {
  const std::filesystem::path rulesFilePath{
      "/mnt/d/Projects/compilers-project/resources/rules.txt"};
  LanguageRulesReader rulesReader(rulesFilePath);

  const std::filesystem::path programFilePath{
      "/mnt/d/Projects/compilers-project/resources/program.txt"};
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
}