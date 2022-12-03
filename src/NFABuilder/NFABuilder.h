#pragma once
#include <LanguageTokens/Token.h>
#include <NFA/NFA.h>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

class NFABuilder {

 public:
  NFABuilder(const std::unordered_map<std::string, std::vector<Token>>
                 regexExpressions,
             const std::vector<std::string> keywords,
             const std::vector<char> punctuationCharacters)
      : regexExpressions_(regexExpressions),
        keywords_(keywords),
        punctuationCharacters_(punctuationCharacters) {}

 private:
  void buildKeywordNFAs() {
    std::vector<std::unique_ptr<NFA>> keywordNFAs(keywords_.size());
    std::transform(keywords_.begin(), keywords_.end(), keywordNFAs.begin(),
                   NFA::constructKeywordNFA);
    for (int i = 1; i < keywordNFAs.size(); i++) {
      keywordNFAs[0]->unite(std::move(keywordNFAs[i]));
    }
  }

  const std::unordered_map<std::string, std::vector<Token>> regexExpressions_;
  const std::vector<std::string> keywords_;
  const std::vector<char> punctuationCharacters_;
};