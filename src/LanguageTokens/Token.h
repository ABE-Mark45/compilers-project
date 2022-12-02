#pragma once
#include <string>
#include <variant>

enum TokenType {
  PLUS,
  ASTERIK,
  LEFT_P,
  RIGHT_P,
  CONCAT,
  OR,
  KEYWORD,
  CHAR_GROUP,
  CHAR,
  EPSILON
};

struct Token {
  TokenType type;
  std::variant<std::monostate, std::pair<char, char>, std::string, char> data;

  Token(TokenType type,
        std::variant<std::monostate, std::pair<char, char>, std::string, char>
            data = {})
      : type(type), data(data) {}

  bool operator==(const Token& o) const {
    return this->type == o.type && this->data == o.data;
  }
};
