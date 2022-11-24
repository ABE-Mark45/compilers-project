#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <vector>
#include "LanguageRulesParser/LanguageRulesParser.h"
#include "LanguageTokens/Token.h"

using std::string;
using namespace Token;

TEST(LanguageRulesParserTest, LanguageRulesParser) {
  std::string line = "letter=[a-z]|[A-Z]";
  std::string digit = "digit=[0-9]";
  std::string identifier = "id=letter(letter|digit+)*";

  std::vector<TokenType> trueIdTokens{
      MetaCharacter('('),       CharacterGroup('a', 'z'), MetaCharacter('|'),
      CharacterGroup('A', 'Z'), MetaCharacter(')'),       MetaCharacter('('),
      MetaCharacter('('),       CharacterGroup('a', 'z'), MetaCharacter('|'),
      CharacterGroup('A', 'Z'), MetaCharacter(')'),       MetaCharacter('|'),
      MetaCharacter('('),       CharacterGroup('0', '9'), MetaCharacter(')'),
      MetaCharacter('+'),       MetaCharacter(')'),       MetaCharacter('*')};

  LanguageRulesParser p;
  p.parseLine(line);
  p.parseLine(digit);
  p.parseLine(identifier);

  auto regexDefinitions = p.getRegexDefinitions();
  const auto& idTokens = regexDefinitions.at("id");

  ASSERT_EQ(idTokens, trueIdTokens);

  p.show();
}