#include <NFA/NFA.h>

NFA::NFA(std::optional<std::string> acceptValue)
    : startState_(std::make_shared<State>()),
      endState_(std::make_shared<State>(acceptValue)) {}

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
    const std::string& keyword) {
  auto nfa = std::make_unique<NFA>();
  auto prev = nfa->startState_;

  for (char c : keyword) {
    auto newState = std::make_shared<State>();
    prev->addTransition(c, newState);
    prev = newState;
  }

  prev->setAcceptValue(keyword);

  nfa->endState_ = prev;

  return nfa;
}

/*static*/ std::unique_ptr<NFA> NFA::constructPunctuationCharacterNFA(
    char punctuationCharacter) {
  auto nfa = std::make_unique<NFA>(std::string(1, punctuationCharacter));
  nfa->startState_->addTransition(punctuationCharacter, nfa->endState_);
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