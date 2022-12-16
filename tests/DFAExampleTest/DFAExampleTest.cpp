#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include <vector>
#include "DFABuilder/DFABuilder.h"
#include "LanguageRulesParser/LanguageRulesParser.h"
#include "NFABuilder/NFABuilder.h"
#include "Readers/LanguageRulesReader.h"
#include "Readers/ProgramReader.h"
#include "Simulator/Simulator.h"

// Declaration (Header):

// standard C++ header:
#include <string>

// returns file path of this executable
std::string getExecPath();

/**************************************************************************/

// Definition (C++ Source):

// standard C++ header:
#include <codecvt>
#include <cstring>
#include <locale>

// OS header:
#ifdef _WIN32  // Is this Windows?
#include <windows.h>
#else  // (not) _WIN32 // Then it's hopefully Linux.
#include <unistd.h>
#endif  // _WIN32

TEST(DFAExampleTest, test) {
  const std::filesystem::path rulesFilePath{"../../../resources/rules.txt"};
  LanguageRulesReader rulesReader(rulesFilePath);

  const std::filesystem::path programFilePath{"../../../resources/program.txt"};
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

  s.finishSimulation();
}

TEST(DFAExampleTest, errorRecovery) {
  const std::filesystem::path rulesFilePath{"../../../resources/rules.txt"};
  LanguageRulesReader rulesReader(rulesFilePath);

  const std::filesystem::path programFilePath{
      "../../../resources/program2.txt"};
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

  std::stringstream ss;
  Simulator s(dfaStartState, ss);

  while (programReader.hasChar()) {
    s.consumeCharacter(programReader.getChar());
  }

  s.finishSimulation();

  ASSERT_EQ(ss.str(), "if\nNo matched token for #\nid\n");
}