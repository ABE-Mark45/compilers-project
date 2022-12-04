#include <NFABuilder/NFABuilder.h>

namespace nfa {
NFABuilder::NFABuilder(
    const std::unordered_map<std::string, std::vector<Token>>&
        postfixRegexExpressions,
    const std::vector<std::string>& keywords,
    const std::vector<char>& punctuationCharacters,
    const std::unordered_map<std::string, int>& priorities)
    : combinedStartState_(std::make_shared<State>()) {
  buildKeywordNFAs(keywords, priorities);
  buildPunctuationNFAs(punctuationCharacters, priorities);
  buildRegexExpressionNFAs(postfixRegexExpressions, priorities);
}

std::unique_ptr<NFA> NFABuilder::getCombinedNFA() {
  auto nfa = std::make_unique<NFA>(combinedStartState_);
  return nfa;
}

void NFABuilder::buildKeywordNFAs(
    const std::vector<std::string>& keywords,
    const std::unordered_map<std::string, int>& priorities) {
  for (const auto& keyword : keywords) {
    int priority = priorities.at(keyword);
    auto nfa = NFA::constructKeywordNFA(keyword, priority);

    combinedStartState_->addEpsilonTransition(nfa->getStartState());
  }
}

void NFABuilder::buildPunctuationNFAs(
    const std::vector<char>& punctuationCharacters,
    const std::unordered_map<std::string, int>& priorities) {
  for (char c : punctuationCharacters) {
    int priority = priorities.at(std::string(1, c));
    auto nfa = NFA::constructPunctuationCharacterNFA(c, priority);

    combinedStartState_->addEpsilonTransition(nfa->getStartState());
  }
}

void NFABuilder::buildRegexExpressionNFAs(
    const std::unordered_map<std::string, std::vector<Token>>&
        postfixRegexExpressions,
    const std::unordered_map<std::string, int>& priorities) {
  for (const auto& [identifier, tokens] : postfixRegexExpressions) {
    int priority = priorities.at(identifier);

    auto nfa = NFA::constructRegexExpressionNFA(identifier, priority, tokens);

    combinedStartState_->addEpsilonTransition(nfa->getStartState());
  }
}

}  // namespace nfa