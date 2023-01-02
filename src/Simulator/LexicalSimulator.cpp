#include <Simulator/LexicalSimulator.h>
#include <optional>
#include <string>
#include <vector>

LexicalSimulator::LexicalSimulator(std::shared_ptr<const dfa::State> startState)
    : startState_(startState),
      currentState_(startState),
      lastAcceptingState_(nullptr) {}

void LexicalSimulator::consumeCharacter(char character) {
  if (std::isspace(character) && currentState_ == startState_) {
    return;
  }
  buffer.push_back(character);
  auto nextState = currentState_->moveThrough(character);
  if (nextState) {
    currentState_ = nextState;
    // save last accepting value
    if (currentState_->getAcceptValue()) {
      lastAcceptingState_ = currentState_;
      buffer.clear();
    }
    return;
  } else {
    flushLastAcceptingState();
  }
}

void LexicalSimulator::flushLastAcceptingState() {
  if (lastAcceptingState_) {
    // flush last accepting state and consume succeeding characters
    tokens.push_back(lastAcceptingState_->getAcceptValue().value().value);
    lastAcceptingState_.reset();
    currentState_ = startState_;
    std::vector<char> newInput = std::move(buffer);
    for (char c : newInput) {
      consumeCharacter(c);
    }
  } else if (!buffer.empty()) {
    auto errorMessage =
        "No matched token for " + std::string(buffer.begin(), buffer.end());
    errors.emplace_back(errorMessage);
    buffer.clear();
  }
}

void LexicalSimulator::finishSimulation() {
  while (lastAcceptingState_) {
    flushLastAcceptingState();
  }

  // are there characters that couldn't be analyzed?
  if (!buffer.empty()) {
    auto errorMessage =
        "No matched token for " + std::string(buffer.begin(), buffer.end());
    errors.emplace_back(errorMessage);
    buffer.clear();
  }
}

std::vector<std::string> LexicalSimulator::getErrors() {
  return vector<string>(errors);
}

std::vector<std::string> LexicalSimulator::getTokens() {
  return vector<string>(tokens);
}

