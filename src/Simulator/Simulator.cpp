#include <Simulator/Simulator.h>
#include <optional>
#include <string>

Simulator::Simulator(std::shared_ptr<const dfa::State> startState,
                     std::ostream& o)
    : startState_(startState), currentState_(startState), o_(o) {}

void Simulator::consumeCharacter(char character) {
  if (std::isspace(character) && currentState_ == startState_) {
    return;
  }
  auto nextState = currentState_->moveThrough(character);
  if (nextState) {
    currentState_ = nextState;
    if(currentState_->getAcceptValue()) {
      last_accepting_state = currentState_;
    }
    return;
  } else if (currentState_->getAcceptValue()) {
    o_ << currentState_->getAcceptValue().value().value << '\n';
    currentState_ = startState_;
    consumeCharacter(character);
  } else {
    throw std::runtime_error("No matched prefix");
  }
}