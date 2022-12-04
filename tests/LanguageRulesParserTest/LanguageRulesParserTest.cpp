#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <vector>
#include "LanguageRulesParser/LanguageRulesParser.h"
#include "LanguageTokens/Token.h"

TEST(LanguageRulesParserTest, LanguageRulesParser) {
  std::string line = "letter= [a-z] | [A-Z]";
  std::string digit = "digit=[0-9] [A-X]";
  std::string identifier = "id: letter ( letter | digit+ \\L [3-5] )*";

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
      Token(TokenType::EPSILON, 0),
      Token(TokenType::CONCAT),
      Token(TokenType::CHAR_GROUP, std::make_pair('3', '5')),
      Token(TokenType::RIGHT_P),
      Token(TokenType::ASTERIK)};

  LanguageRulesParser p;
  p.parseLine(line);
  p.parseLine(digit);
  p.parseLine(identifier);

  auto regexExpressions = p.getRegexExpressions();
  const auto& idTokens = regexExpressions.at("id");

  ASSERT_EQ(idTokens.size(), trueIdTokens.size());
  ASSERT_EQ(idTokens, trueIdTokens);

  p.show();

  auto postfix = p.infixToPostfix(idTokens);
  p.printTokensVector(postfix);
}

TEST(LanguageRulesParserTest, ParseKeywords) {
  const std::string keywordsString = "{one two three    if else then}";
  const std::vector<std::string> trueKeywords{"one", "two",  "three",
                                              "if",  "else", "then"};
  LanguageRulesParser p;

  p.parseLine(keywordsString);

  const auto& keywords = p.getKeywords();

  ASSERT_EQ(keywords, trueKeywords);
}

TEST(LanguageRulesParserTest, ParsePunctuation) {
  const std::string punctuationCharactersString = "[; , \\( \\) { }]";
  const std::vector<char> truePunctuationCharacters{';', ',', '(',
                                                    ')', '{', '}'};
  LanguageRulesParser p;

  p.parseLine(punctuationCharactersString);

  const auto& punctuationCharacters = p.getPunctuationCharacters();

  ASSERT_EQ(punctuationCharacters, truePunctuationCharacters);
}

TEST(LanguageRulesParserTest, JavaExample) {
  const std::string letter = "letter= [a-z] | [A-Z]";
  const std::string digit = "digit= [0-9]";
  const std::string id = "id: letter (letter | digit)*";
  const std::string digits = "digits= digit+";
  const std::string keywords1 = "{boolean int  float }";
  const std::string num = "num: digit+ | digit+ . digits ( \\L | E digits)";
  const std::string relop = "relop: \\=\\= | !\\= | > | >\\= | < | <\\=";
  const std::string assign = "assign: \\=";
  const std::string keywords2 = "{if else while}";
  const std::string punctuation = "[; ,  \\( \\) { }]";
  const std::string addop = "addop: \\+ | \\-";
  const std::string mulop = "mulop: \\* | \\/";

  const std::vector<std::string> toBeParsedStrings{
      letter, digit,  id,        digits,      keywords1, num,
      relop,  assign, keywords2, punctuation, addop,     mulop};

  LanguageRulesParser p;
  for (const auto& str : toBeParsedStrings) {
    p.parseLine(str);
  }

  const std::vector<char> truePunctuationCharacters{';', ',', '(',
                                                    ')', '{', '}'};

  const std::vector<std::string> trueKeywords{"boolean", "int",  "float",
                                              "if",      "else", "while"};

  const std::vector<Token> trueId{
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
      Token(TokenType::RIGHT_P),
      Token(TokenType::RIGHT_P),
      Token(TokenType::ASTERIK)};

  const std::vector<Token> trueRelop{
      Token(TokenType::CHAR, '='), Token(TokenType::CONCAT),
      Token(TokenType::CHAR, '='), Token(TokenType::OR),
      Token(TokenType::CHAR, '!'), Token(TokenType::CONCAT),
      Token(TokenType::CHAR, '='), Token(TokenType::OR),
      Token(TokenType::CHAR, '>'), Token(TokenType::OR),
      Token(TokenType::CHAR, '>'), Token(TokenType::CONCAT),
      Token(TokenType::CHAR, '='), Token(TokenType::OR),
      Token(TokenType::CHAR, '<'), Token(TokenType::OR),
      Token(TokenType::CHAR, '<'), Token(TokenType::CONCAT),
      Token(TokenType::CHAR, '=')};

  const std::vector<Token> trueNum{
      Token(TokenType::LEFT_P),
      Token(TokenType::CHAR_GROUP, std::make_pair('0', '9')),
      Token(TokenType::RIGHT_P),
      Token(TokenType::PLUS),
      Token(TokenType::OR),
      Token(TokenType::LEFT_P),
      Token(TokenType::CHAR_GROUP, std::make_pair('0', '9')),
      Token(TokenType::RIGHT_P),
      Token(TokenType::PLUS),
      Token(TokenType::CONCAT),
      Token(TokenType::CHAR, '.'),
      Token(TokenType::CONCAT),
      Token(TokenType::LEFT_P),
      Token(TokenType::LEFT_P),
      Token(TokenType::CHAR_GROUP, std::make_pair('0', '9')),
      Token(TokenType::RIGHT_P),
      Token(TokenType::PLUS),
      Token(TokenType::RIGHT_P),
      Token(TokenType::CONCAT),
      Token(TokenType::LEFT_P),
      Token(TokenType::EPSILON, 0),
      Token(TokenType::OR),
      Token(TokenType::CHAR, 'E'),
      Token(TokenType::CONCAT),
      Token(TokenType::LEFT_P),
      Token(TokenType::LEFT_P),
      Token(TokenType::CHAR_GROUP, std::make_pair('0', '9')),
      Token(TokenType::RIGHT_P),
      Token(TokenType::PLUS),
      Token(TokenType::RIGHT_P),
      Token(TokenType::RIGHT_P)};

  const std::vector<Token> trueAssign{Token(TokenType::CHAR, '=')};

  const std::vector<Token> trueAddop{Token(TokenType::CHAR, '+'),
                                     Token(TokenType::OR),
                                     Token(TokenType::CHAR, '-')};

  const std::vector<Token> trueMulop{Token(TokenType::CHAR, '*'),
                                     Token(TokenType::OR),
                                     Token(TokenType::CHAR, '/')};

  auto regexExpressions = p.getRegexExpressions();
  auto keywords = p.getKeywords();
  auto puctuationCharacters = p.getPunctuationCharacters();

  ASSERT_EQ(regexExpressions["id"], trueId);
  ASSERT_EQ(regexExpressions["relop"], trueRelop);
  ASSERT_EQ(regexExpressions["num"], trueNum);
  ASSERT_EQ(regexExpressions["assign"], trueAssign);
  ASSERT_EQ(regexExpressions["addop"], trueAddop);
  ASSERT_EQ(regexExpressions["mulop"], trueMulop);
  ASSERT_EQ(puctuationCharacters, truePunctuationCharacters);
  ASSERT_EQ(keywords, trueKeywords);

  p.printTokensVector(p.getPostfixRegexExpressions().at("num"));
}