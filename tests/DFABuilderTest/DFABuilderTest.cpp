#include <gtest/gtest.h>
#include <iostream>
#include <DFABuilder/DFABuilder.h>
#include "LanguageTokens/Token.h"
#include "NFA/NFA.h"
#include "NFABuilder/NFABuilder.h"
#include "DFABuilder/DFABuilder.h"
#include "LanguageRulesParser/LanguageRulesParser.h"

TEST(DFABuilderTest, test1) {
  std::string line = "letter= [a-z] | [A-Z]";
  std::string digit = "digit=[0-9] [A-X]";
  std::string identifier = "id: letter ( letter | digit+ \\L [3-5] )*";

  LanguageRulesParser p;
  p.parseLine(line);
  p.parseLine(digit);
  p.parseLine(identifier);

  // auto nfaBuilder = NFABuilder(
  //   p.getPostfixRegexExpressions(),
  //   p.getKeywords(),
  //   p.getPunctuationCharacters(),
  //   p.getPriorities(),
  // ).getCombinedNFA();

  // auto nfa = nfaBuilder.getCombinedNFA();
  // DFABuilder dfaBuilder = DFABuilder();
  // dfaBuilder.buildDFA(std::move(nfa));

}
