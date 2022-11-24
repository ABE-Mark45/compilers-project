#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <vector>
#include "LanguageRulesParser/LanguageRulesParser.h"
#include "LanguageTokens/Token.h"

TEST(LanguageRulesParserTest, LanguageRulesParser) {
  std::string line = "letter=[a-z]|[A-Z]";
  std::string digit = "digit=[0-9][A-X]";
  std::string identifier = "id=letter(letter|digit+\\L[3-5])*";

  std::vector<Token> trueIdTokens{
      Token(TokenType::LEFT_P),
      Token(TokenType::CHAR_GROUP, std::make_pair('a', 'z')),
      Token(TokenType::OR),
      Token(TokenType::CHAR_GROUP, std::make_pair('A', 'Z')),
      Token(TokenType::RIGHT_P),
      Token(TokenType::CONCAT),
      Token(TokenType::LEFT_P),
      Token(TokenType::LEFT_P),
      Token(TokenType::CHAR_GROUP, std::make_pair('a', 'z')),
      Token(TokenType::OR),
      Token(TokenType::CHAR_GROUP, std::make_pair('A', 'Z')),
      Token(TokenType::RIGHT_P),
      Token(TokenType::OR),
      Token(TokenType::LEFT_P),
      Token(TokenType::CHAR_GROUP, std::make_pair('0', '9')),
      Token(TokenType::CONCAT),
      Token(TokenType::CHAR_GROUP, std::make_pair('A', 'X')),
      Token(TokenType::RIGHT_P),
      Token(TokenType::PLUS),
      Token(TokenType::CONCAT),
      Token(TokenType::RES_CHAR, 'L'),
      Token(TokenType::CONCAT),
      Token(TokenType::CHAR_GROUP, std::make_pair('3', '5')),
      Token(TokenType::RIGHT_P),
      Token(TokenType::ASTERIK)};

  LanguageRulesParser p;
  p.parseLine(line);
  p.parseLine(digit);
  p.parseLine(identifier);

  auto regexDefinitions = p.getRegexDefinitions();
  const auto& idTokens = regexDefinitions.at("id");

  ASSERT_EQ(idTokens.size(), trueIdTokens.size());
  ASSERT_EQ(idTokens, trueIdTokens);

  p.show();

  auto postfix = p.infixToPostfix(idTokens);
  p.printTokensVector(postfix);
}