#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <DFABuilder/DFABuilder.h>
#include "LanguageTokens/Token.h"
#include "NFA/NFA.h"
#include "NFABuilder/NFABuilder.h"
#include "DFABuilder/DFABuilder.h"
#include "LanguageRulesParser/LanguageRulesParser.h"
#include "Simulator/Simulator.h"

using namespace std;

TEST(DFABuilderTest, testMinimization1) {
  // sorry there is no asserts atm.
  // verify the output for now.

  // https://www.youtube.com/watch?v=0XaGAkY09Wc&ab_channel=NesoAcademy has the test example

  shared_ptr<dfa::State> A = std::make_shared<dfa::State>();
  shared_ptr<dfa::State> B = std::make_shared<dfa::State>();
  shared_ptr<dfa::State> C = std::make_shared<dfa::State>();
  shared_ptr<dfa::State> D = std::make_shared<dfa::State>();
  shared_ptr<dfa::State> E = std::make_shared<dfa::State>(AcceptValue(1, "done"));
  A->addTransition(1,B);
  A->addTransition(2,C);
  B->addTransition(1,B);
  B->addTransition(2,D);
  C->addTransition(1,B);
  C->addTransition(2,C);
  D->addTransition(1,B);
  D->addTransition(2,E);
  E->addTransition(1,B);
  E->addTransition(2,C);

  DFABuilder::minimizeDFA(A);

  /*
5 -> 7,6
6 -> 7,6
7 -> 7,8
8 -> 7,5
size: 4  
  */
}

TEST(DFABuilderTest, testMinimization2) {
  // https://www.youtube.com/watch?v=ex9sPLq5CRg&ab_channel=NesoAcademy has the test example
  shared_ptr<dfa::State> Q0 = std::make_shared<dfa::State>();
  shared_ptr<dfa::State> Q1 = std::make_shared<dfa::State>();
  shared_ptr<dfa::State> Q2 = std::make_shared<dfa::State>(AcceptValue(1, "done"));
  shared_ptr<dfa::State> Q3 = std::make_shared<dfa::State>();
  shared_ptr<dfa::State> Q4 = std::make_shared<dfa::State>();
  shared_ptr<dfa::State> Q5 = std::make_shared<dfa::State>();
  shared_ptr<dfa::State> Q6 = std::make_shared<dfa::State>();
  shared_ptr<dfa::State> Q7 = std::make_shared<dfa::State>();
  Q0->addTransition(1,Q1);
  Q0->addTransition(2,Q5);
  Q1->addTransition(1,Q6);
  Q1->addTransition(2,Q2);
  Q2->addTransition(1,Q0);
  Q2->addTransition(2,Q2);
  Q3->addTransition(1,Q2);
  Q3->addTransition(2,Q6);
  Q4->addTransition(1,Q7);
  Q4->addTransition(2,Q5);
  Q5->addTransition(1,Q2);
  Q5->addTransition(2,Q6);
  Q6->addTransition(1,Q6);
  Q6->addTransition(2,Q4);
  Q7->addTransition(1,Q6);
  Q7->addTransition(2,Q2);

  DFABuilder::minimizeDFA(Q0);
  /*
17 -> 18,17
18 -> 20,21
19 -> 19,18
20 -> 19,17
21 -> 17,19
size: 5
*/
}



// a general test!
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
