#pragma once
#include <DFA/State.h>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "CFGParser/CFGParser.h"

using namespace std;

class LexicalSimulator {
 public:
  explicit LexicalSimulator(std::shared_ptr<const dfa::State> startState);
  LexicalSimulator() = delete;

  void consumeCharacter(char character);

  void finishSimulation();

  std::vector<std::string> getErrors();
  std::vector<std::string> getTokens();
 private:
  void flushLastAcceptingState();

  std::shared_ptr<const dfa::State> startState_;
  std::shared_ptr<const dfa::State> currentState_;
  std::shared_ptr<const dfa::State> lastAcceptingState_;
  std::vector<char> buffer;
  std::vector<std::string> errors;
  std::vector<string> tokens;
};

