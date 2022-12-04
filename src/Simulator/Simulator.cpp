#include <Simulator/Simulator.h>
#include <iostream>
#include <optional>
#include <string>

Simulator::Simulator(std::shared_ptr<const dfa::State> startState)
    : startState_(startState), currentState_(startState) {}

void Simulator::consumeCharacter(char character) {
  if (std::isspace(character) && currentState_ == startState_) {
    return;
  }
  auto nextState = currentState_->moveThrough(character);
  if (nextState) {
    currentState_ = nextState;
    return;
  } else if (currentState_->getAcceptValue()) {
    std::cout << currentState_->getAcceptValue().value().value << std::endl;
    currentState_ = startState_;
    consumeCharacter(character);
  } else {
    throw std::runtime_error("No matched prefix");
  }
}