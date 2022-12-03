#pragma once
#include <LanguageTokens/Token.h>
#include <NFA/NFA.h>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

class NFABuilder {

 public:
  NFABuilder(const std::unordered_map<std::string, std::vector<Token>>&
                 postfixRegexExpressions,
             const std::vector<std::string>& keywords,
             const std::vector<char>& punctuationCharacters,
             const std::unordered_map<std::string, int>& priorities);

  std::unique_ptr<NFA> getCombinedNFA();

 private:
  void buildKeywordNFAs();

  void buildPunctuationNFAs();

  void buildRegexExpressionNFAs();

  const std::unordered_map<std::string, std::vector<Token>>
      postfixRegexExpressions_;
  const std::vector<std::string> keywords_;
  const std::vector<char> punctuationCharacters_;
  const std::unordered_map<std::string, int> priorities_;
  std::shared_ptr<State> combinedStartState_;
};