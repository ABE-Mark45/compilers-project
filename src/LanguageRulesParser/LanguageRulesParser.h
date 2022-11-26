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

  // gets a const reference to the regex definitions
  const std::unordered_map<std::string, std::vector<Token>>&
  getRegexDefinitions() {
    return regexDefinitions;
  }

  std::vector<Token> infixToPostfix(const std::vector<Token>& tokens) const;

 private:
  // Parse a definition line on the form <identifier>=<regex>
  void parseDef(const std::string& identifier, std::string_view line, int idx);

  // A helper function to control whether to parse a regex definition or expression
  void parseDefOrExp(std::string_view line);

  // A helper function to indicate if concatenation should be added
  bool shouldConcatenate(const std::vector<Token>& tokens) const;

  int tokenPrecedence(const TokenType type) const;
  bool compareTokensByType(const Token& a, const Token& b) const;

  std::unordered_map<std::string, std::vector<Token>> regexDefinitions;
};