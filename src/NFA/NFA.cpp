#include <NFA/NFA.h>
#include <stack>

namespace nfa {
NFA::NFA(std::shared_ptr<State> startState, std::shared_ptr<State> endState)
    : startState_(startState), endState_(endState) {}

/*static*/ std::unique_ptr<NFA> NFA::constructCharacterGroupNFA(
    const Token& token) {
  auto startState = std::make_shared<State>();
  auto endState = std::make_shared<State>();

  auto [beginCharacter, endCharacter] =
      std::get<std::pair<char, char>>(token.data);

  for (char c = beginCharacter; c <= endCharacter; c++) {
    startState->addTransition(c, endState);
  }

  auto nfa = std::make_unique<NFA>(startState, endState);
  return nfa;
}

/*static*/ std::unique_ptr<NFA> NFA::constructKeywordNFA(
    const std::string& keyword, int priority) {
  auto startState = std::make_shared<State>();
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
      AcceptValue{priority, std::string(1, punctuationCharacter)});
  startState->addTransition(punctuationCharacter, endState);

  auto nfa = std::make_unique<NFA>(startState, endState);
  return nfa;
}

/*static*/ std::unique_ptr<NFA> NFA::constructCharacterNFA(const Token& token) {
  auto startState = std::make_shared<State>();
  auto endState = std::make_shared<State>();

  char character = std::get<char>(token.data);

  startState->addTransition(character, endState);

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

/*static*/ std::unique_ptr<NFA> NFA::constructRegexExpressionNFA(
    const std::string& identifier, int priority,
    const std::vector<Token>& tokens) {
  std::stack<std::unique_ptr<NFA>> nfaStack;

  for (const auto& token : tokens) {
    switch (token.type) {
      case CHAR_GROUP: {
        auto nfa = constructCharacterGroupNFA(token);
        nfaStack.push(std::move(nfa));
        break;
      }
      case EPSILON:
      case CHAR: {
        auto nfa = constructCharacterNFA(token);
        nfaStack.push(std::move(nfa));
        break;
      }
      case OR: {
        auto secondNFA = std::move(nfaStack.top());
        nfaStack.pop();
        auto firstNFA = std::move(nfaStack.top());
        nfaStack.pop();
        firstNFA->unite(std::move(secondNFA));
        nfaStack.push(std::move(firstNFA));
        break;
      }
      case CONCAT: {
        auto secondNFA = std::move(nfaStack.top());
        nfaStack.pop();
        auto firstNFA = std::move(nfaStack.top());
        nfaStack.pop();
        firstNFA->concatenate(std::move(secondNFA));
        nfaStack.push(std::move(firstNFA));
        break;
      }
      case PLUS: {
        nfaStack.top()->positiveKleeneStar();
        break;
      }
      case ASTERIK: {
        nfaStack.top()->kleeneStar();
        break;
      }
      default: {
        throw std::runtime_error("Unsupported token");
      }
    }
  }

  if (nfaStack.size() != 1) {
    throw std::runtime_error("Stack should only contain one NFA");
  }
  auto combinedNFA = std::move(nfaStack.top());
  nfaStack.pop();
  combinedNFA->endState_->setAcceptValue({priority, identifier});

  return combinedNFA;
}

}