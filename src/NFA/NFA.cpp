#include <NFA/NFA.h>

NFA::NFA(std::shared_ptr<State> startState, std::shared_ptr<State> endState)
    : startState_(startState), endState_(endState) {}

/*static*/ std::unique_ptr<NFA> NFA::constructCharacterGroupNFA(
    const Token& token) {
  auto nfa = std::make_unique<NFA>();

  auto [beginCharacter, endCharacter] =
      std::get<std::pair<char, char>>(token.data);

  for (char c = beginCharacter; c <= endCharacter; c++) {
    nfa->startState_->addTransition(c, nfa->endState_);
  }

  return nfa;
}

/*static*/ std::unique_ptr<NFA> NFA::constructKeywordNFA(
    const std::string& keyword, int priority) {
  auto startState = std::shared_ptr<State>();
  auto currentState = startState;

  for (char c : keyword) {
    auto newState = std::make_shared<State>();
    currentState->addTransition(c, newState);
    currentState = newState;
  }

  currentState->setAcceptValue({priority, keyword});

  auto nfa = std::make_unique<NFA>(startState, currentState);

  return nfa;
}

/*static*/ std::unique_ptr<NFA> NFA::constructPunctuationCharacterNFA(
    char punctuationCharacter, int priority) {
  auto startState = std::make_shared<State>();
  auto endState = std::make_shared<State>(
      State::AcceptValue{priority, std::string(1, punctuationCharacter)});
  startState->addTransition(punctuationCharacter, endState);

  auto nfa = std::make_unique<NFA>(startState, endState);
  return nfa;
}

void NFA::concatenate(std::shared_ptr<NFA> otherNFA) {
  endState_->addEpsilonTransition(otherNFA->startState_);
  endState_ = std::move(otherNFA->endState_);
}

void NFA::unite(std::shared_ptr<NFA> otherNFA) {
  auto newStartState = std::make_shared<State>();
  auto newEndState = std::make_shared<State>();

  newStartState->addEpsilonTransition(startState_);
  newStartState->addEpsilonTransition(otherNFA->startState_);

  endState_->addEpsilonTransition(newEndState);
  otherNFA->endState_->addEpsilonTransition(newEndState);

  startState_ = std::move(newStartState);
  endState_ = std::move(newEndState);
}

void NFA::kleeneStar() {
  auto newStartState = std::make_shared<State>();
  auto newEndState = std::make_shared<State>();

  newStartState->addEpsilonTransition(startState_);
  endState_->addEpsilonTransition(newEndState);

  newStartState->addEpsilonTransition(newEndState);
  endState_->addEpsilonTransition(startState_);

  startState_ = std::move(newStartState);
  endState_ = std::move(newEndState);
}

void NFA::positiveKleeneStar() {
  auto newStartState = std::make_shared<State>();
  auto newEndState = std::make_shared<State>();

  newStartState->addEpsilonTransition(startState_);
  endState_->addEpsilonTransition(newEndState);

  endState_->addEpsilonTransition(startState_);

  startState_ = std::move(newStartState);
  endState_ = std::move(newEndState);
}