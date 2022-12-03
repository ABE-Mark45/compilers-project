#pragma once
#include <LanguageTokens/Token.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class LanguageRulesParser {

 public:
  // Interface to parsing logic
  void parseLine(std::string_view line);

  // Debugging function that outputs all parsed tokens
  void show() const;
  void printToken(const Token& token) const;
  void printTokensVector(const std::vector<Token>& tokens) const;

  // getters
  const auto& getRegexExpressions() { return regexExpressions_; }
  const auto& getPostfixRegexExpressions() { return postfixRegexExpressions_; }
  const auto& getKeywords() { return keywords_; }
  const auto& getPunctuationCharacters() { return punctuationCharacters_; }
  const auto& getPriorities() { return priorities_; }

  std::vector<Token> infixToPostfix(const std::vector<Token>& tokens) const;

 private:
  // Parse a definition line on the form <identifier>=<regex>
  std::vector<Token> parseDef(std::string_view line, int idx);

  // A helper function to control whether to parse a regex definition or expression
  void parseDefOrExp(std::string_view line);

  // A helper function to indicate if concatenation should be added
  bool shouldConcatenate(const std::vector<Token>& tokens) const;

  int tokenPrecedence(const TokenType type) const;
  bool compareTokensByType(const Token& a, const Token& b) const;

  void parseKeywords(std::string_view line);
  void parsePunctuation(std::string_view line);

  std::unordered_map<std::string, std::vector<Token>> regexDefinitions_;
  std::unordered_map<std::string, std::vector<Token>> regexExpressions_;
  std::unordered_map<std::string, std::vector<Token>> postfixRegexExpressions_;
  std::unordered_map<std::string, int> priorities_;
  std::vector<std::string> keywords_;
  std::vector<char> punctuationCharacters_;
};