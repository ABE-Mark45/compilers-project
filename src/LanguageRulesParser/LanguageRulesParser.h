#pragma once
#include <LanguageTokens/Token.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using TokenType = std::variant<Token::CharacterGroup, Token::MetaCharacter,
                               Token::Identifier, Token::ReservedCharacter>;
class LanguageRulesParser {

 public:
  // Interface to parsing logic
  void parseLine(const std::string& line);

  // Debugging function that outputs all parsed tokens
  void show();

  // gets a const reference to the regex definitions
  const std::unordered_map<std::string, std::vector<TokenType>>&
  getRegexDefinitions() {
    return regexDefinitions;
  }

 private:
  // Parse a definition line on the form <identifier>=<regex>
  void parseDef(const std::string& identifier, const std::string& line,
                int idx);

  // A helper function to control whether to parse a regex definition or expression
  void parseDefOrExp(const std::string& line);

  std::unordered_map<std::string, std::vector<TokenType>> regexDefinitions;
};