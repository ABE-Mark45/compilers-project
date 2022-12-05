#pragma once
#include <DFA/State.h>
#include <iostream>
#include <memory>

class Simulator {
 public:
  explicit Simulator(std::shared_ptr<const dfa::State> startState,
                     std::ostream& o = std::cout);
  Simulator() = delete;

  void consumeCharacter(char character);

  void finishSimulation();

 private:

  void flushLastAcceptingState();

  std::shared_ptr<const dfa::State> startState_;
  std::shared_ptr<const dfa::State> currentState_;
  std::ostream& o_;
  std::shared_ptr<const dfa::State> lastAcceptingState_;
  std::vector<char> buffer;
};