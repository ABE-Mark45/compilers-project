#include <Simulator/Simulator.h>
#include <optional>
#include <string>
#include <vector>

Simulator::Simulator(std::shared_ptr<const dfa::State> startState,
                     std::ostream& o)
    : startState_(startState), currentState_(startState), o_(o), lastAcceptingState_(nullptr) {}

void Simulator::consumeCharacter(char character) {
  if (std::isspace(character) && currentState_ == startState_) {
    return;
  }
  buffer.push_back(character);
  auto nextState = currentState_->moveThrough(character);
  if (nextState) {
    currentState_ = nextState;
    // save last accepting value
    if(currentState_->getAcceptValue()) {
      lastAcceptingState_ = currentState_;
      buffer.clear();
    }
    return;
  } else {
    flushLastAcceptingState();
  }
}

void Simulator::flushLastAcceptingState() {
  if (lastAcceptingState_) {
    // flush last accepting state and consume succeeding characters
    o_ << lastAcceptingState_->getAcceptValue().value().value << '\n';
    lastAcceptingState_.reset();
    currentState_ = startState_;
    std::vector<char> newInput = std::move(buffer);
    for(char c: newInput){
      consumeCharacter(c);
    }
  } else if(!buffer.empty()){
    errors.emplace_back("No matched token for " + std::string(buffer.begin(), buffer.end()));
    buffer.clear();
  }
}

void Simulator::finishSimulation() {
  while (lastAcceptingState_) {
    flushLastAcceptingState();
  }

  // are there characters that couldn't be analyzed?
  if(!buffer.empty()) {
    errors.emplace_back("No matched token for " + std::string(buffer.begin(), buffer.end()));
    buffer.clear();
  }
}

std::vector<std::string> Simulator::getErrors(){
  return errors;
} 