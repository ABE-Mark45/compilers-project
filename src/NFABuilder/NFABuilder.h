#pragma once
#include <LanguageTokens/Token.h>
#include <NFA/NFA.h>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

namespace nfa {
class NFABuilder {

 public:
  NFABuilder(const std::unordered_map<std::string, std::vector<Token>>&
                 postfixRegexExpressions,
             const std::vector<std::string>& keywords,
             const std::vector<char>& punctuationCharacters,
             const std::unordered_map<std::string, int>& priorities);
  NFABuilder() = delete;

  std::unique_ptr<NFA> getCombinedNFA();

 private:
  void buildKeywordNFAs(const std::vector<std::string>& keywords,
                        const std::unordered_map<std::string, int>& priorities);

  void buildPunctuationNFAs(
      const std::vector<char>& punctuationCharacters,
      const std::unordered_map<std::string, int>& priorities);

  void buildRegexExpressionNFAs(
      const std::unordered_map<std::string, std::vector<Token>>&
          postfixRegexExpressions,
      const std::unordered_map<std::string, int>& priorities);

  std::shared_ptr<State> combinedStartState_;
};

}  // namespace nfa