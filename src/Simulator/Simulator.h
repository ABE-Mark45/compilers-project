#pragma once
#include <DFA/State.h>
#include <memory>

class Simulator {
 public:
  explicit Simulator(std::shared_ptr<const dfa::State> startState);
  Simulator() = delete;

  void consumeCharacter(char character);

 private:
  std::shared_ptr<const dfa::State> startState_;
  std::shared_ptr<const dfa::State> currentState_;
};