#pragma once
#include <DFA/State.h>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "CFGParser/CFGParser.h"

using namespace std;

using NT = string;  // Non-Terminal Token

const string END_INPUT_TOKEN = "$";
const string SYNC_TOKEN = "#";

class ParserSimulator {
 public:
  explicit ParserSimulator(map<pair<NT, string>, ProductionContent> table,
                           ProductionToken topNT);
  ParserSimulator() = delete;

  void consumeToken(string token);

  vector<string> getErrors();

 private:
  void printProduction(NT nt, ProductionContent pc);
  void printTokens(vector<ProductionToken> v);
  bool isٍٍٍSyncProduction(ProductionContent pc);
  bool isTerminalToken(ProductionToken pt);

  vector<vector<ProductionToken>> history;
  vector<ProductionToken> tokenStack;
  std::vector<std::string> errors;
  map<pair<NT,string>,ProductionContent> table;
};