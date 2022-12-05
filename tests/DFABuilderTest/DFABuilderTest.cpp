#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <DFABuilder/DFABuilder.h>
#include "LanguageTokens/Token.h"
#include "NFA/NFA.h"
#include "NFABuilder/NFABuilder.h"
#include "DFABuilder/DFABuilder.h"
#include "LanguageRulesParser/LanguageRulesParser.h"
#include "Simulator/Simulator.h"

TEST(DFABuilderTest, test1) {
  LanguageRulesParser parser;
  // while (auto rule = rulesReader.getLine()) {
  //   parser.parseLine(rule.value());
  // }
  parser.parseLine("{if el ifelse}");

  nfa::NFABuilder nfaBuilder(
      parser.getPostfixRegexExpressions(), parser.getKeywords(),
      parser.getPunctuationCharacters(), parser.getPriorities());

  auto nfa = nfaBuilder.getCombinedNFA();

  auto dfaStartState =  DFABuilder::buildDFA(std::move(nfa));

  std::stringstream output;
  Simulator s(dfaStartState, output);

  std::string program = "ifel";
  for(char c: program)
    s.consumeCharacter(c);

  s.finishSimulation();

  ASSERT_EQ(output.str(), "if\nel\n");
}
