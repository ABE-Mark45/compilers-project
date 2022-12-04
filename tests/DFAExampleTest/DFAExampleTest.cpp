#include <gtest/gtest.h>
#include <vector>
#include "DFABuilder/DFABuilder.h"
#include "LanguageRulesParser/LanguageRulesParser.h"
#include "NFABuilder/NFABuilder.h"
#include "Simulator/Simulator.h"

TEST(DFAExampleTest, test) {
  const std::vector<std::string> languageRules{
      "letter= [a-z] | [A-Z]",
      "digit= [0-9]",
      "id: letter (letter | digit)*",
      "digits= digit+",
      "{boolean int  float }",
      "num: digit+ | digit+ . digits ( \\L | E digits)",
      "relop: \\=\\= | !\\= | > | >\\= | < | <\\=",
      "assign: \\=",
      "{if else while}",
      "[; ,  \\( \\) { }]",
      "addop: \\+ | \\-",
      "mulop: \\* | \\/"};

  const std::string testProgram =
      "int sum, count, pass, mnt;\n"
      "while ( pass != 10) {\n"
      "\tpass = pass + 1;\n"
      "}\n";

  LanguageRulesParser parser;
  for (const auto& rule : languageRules) {
    parser.parseLine(rule);
  }

  nfa::NFABuilder nfaBuilder(
      parser.getPostfixRegexExpressions(), parser.getKeywords(),
      parser.getPunctuationCharacters(), parser.getPriorities());

  auto nfa = nfaBuilder.getCombinedNFA();

  auto dfaStartState = DFABuilder::buildDFA(std::move(nfa));

  Simulator s(dfaStartState);

  for (char c : testProgram) {
    s.consumeCharacter(c);
  }
}