#pragma once
#include <DFA/State.h>
#include <iostream>
#include <string>
#include <memory>
#include <map>
#include <vector>
#include <string>
#include "CFGParser/CFGParser.h"

using namespace std;

class Simulator {
 public:
  explicit Simulator(std::shared_ptr<const dfa::State> startState);
  Simulator() = delete;

  void consumeCharacter(char character);

  void finishSimulation();

  std::vector<std::string> getErrors();
  std::vector<std::string> getTokens();
 private:

  void flushLastAcceptingState();

  std::shared_ptr<const dfa::State> startState_;
  std::shared_ptr<const dfa::State> currentState_;
  std::ostream& o_;
  std::shared_ptr<const dfa::State> lastAcceptingState_;
  std::vector<char> buffer;
  std::vector<std::string> errors;
  std::vector<string> tokens;
};


using NT = string;  // Non-Terminal Token

const string END_INPUT_TOKEN = "$";
const string SYNC_TOKEN = "#";

class ParserSimulator {
 public:
  explicit ParserSimulator(map<pair<NT,string>,ProductionContent> table, ProductionToken topNT);
  ParserSimulator() = delete;

  void consumeToken(string token);

  void finishSimulation();

  vector<string> getErrors();
 private:
  void printProduction(NT nt, ProductionContent pc);
  void printTokens(vector<ProductionToken> v);
  bool isٍٍٍSyncProduction(ProductionContent pc);
  bool isTerminalToken(ProductionToken pt);
  
  vector<vector<ProductionToken>> history;
  vector<ProductionToken> tokenStack;
  std::vector<std::string> errors;
};