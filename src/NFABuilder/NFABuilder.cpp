#include <NFABuilder/NFABuilder.h>

NFABuilder::NFABuilder(
    const std::unordered_map<std::string, std::vector<Token>>&
        postfixRegexExpressions,
    const std::vector<std::string>& keywords,
    const std::vector<char>& punctuationCharacters,
    const std::unordered_map<std::string, int>& priorities)
    : postfixRegexExpressions_(postfixRegexExpressions),
      keywords_(keywords),
      punctuationCharacters_(punctuationCharacters),
      priorities_(priorities),
      combinedStartState_(std::make_shared<State>()) {
  buildKeywordNFAs();
  buildPunctuationNFAs();
  buildRegexExpressionNFAs();
}

std::unique_ptr<NFA> NFABuilder::getCombinedNFA() {
  auto nfa = std::make_unique<NFA>(combinedStartState_);
  return nfa;
}

void NFABuilder::buildKeywordNFAs() {
  for (const auto& keyword : keywords_) {
    int priority = priorities_.at(keyword);
    auto nfa = NFA::constructKeywordNFA(keyword, priority);

    combinedStartState_->addEpsilonTransition(nfa->getStartState());
  }
}

void NFABuilder::buildPunctuationNFAs() {
  for (char c : punctuationCharacters_) {
    int priority = priorities_.at(std::string(1, c));
    auto nfa = NFA::constructPunctuationCharacterNFA(c, priority);

    combinedStartState_->addEpsilonTransition(nfa->getStartState());
  }
}

void NFABuilder::buildRegexExpressionNFAs() {
  for (const auto& [identifier, tokens] : postfixRegexExpressions_) {
    int priority = priorities_.at(identifier);

    auto nfa = NFA::constructRegexExpressionNFA(identifier, priority, tokens);

    combinedStartState_->addEpsilonTransition(nfa->getStartState());
  }
}
