#include "LanguageRulesParser/LanguageRulesParser.h"

#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace {
std::unordered_set<char> regexCharacters{'*', '|', '+', '.', '(', ')'};
};

bool LanguageRulesParser::shouldConcatenate(
    const std::vector<Token>& tokens) const {
  if (tokens.empty()) {
    return false;
  }

  const auto& token = tokens.back();

  switch (token.type) {
    case TokenType::CHAR_GROUP:
    case TokenType::RES_CHAR:
    case TokenType::RIGHT_P:
    case TokenType::ASTERIK:
    case TokenType::DOT:
    case TokenType::PLUS:
      return true;
    default:
      return false;
  }
}

void LanguageRulesParser::parseDef(const std::string& identifier,
                                   std::string_view line, int idx) {
  int len = line.size();
  std::vector<Token> tokens;

  while (idx < len) {
    if (regexCharacters.count(line[idx])) {
      // TODO: Look for cleaner implementation
      if (line[idx] == '(' && shouldConcatenate(tokens)) {
        tokens.emplace_back(TokenType::CONCAT);
      }

      switch (line[idx]) {
        case '(':
          tokens.emplace_back(TokenType::LEFT_P);
          break;
        case ')':
          tokens.emplace_back(TokenType::RIGHT_P);
          break;
        case '*':
          tokens.emplace_back(TokenType::ASTERIK);
          break;
        case '+':
          tokens.emplace_back(TokenType::PLUS);
          break;
        case '.':
          tokens.emplace_back(TokenType::DOT);
          break;
        case '|':
          tokens.emplace_back(TokenType::OR);
          break;
        default:
          throw std::runtime_error("Unrecognized symbol");
      }

      idx++;
    } else if (line[idx] == '[') {
      if (idx + 4 < len && std::isalnum(line[idx + 1]) &&
          line[idx + 2] == '-' && std::isalnum(line[idx + 3]) &&
          line[idx + 4] == ']' && line[idx + 1] <= line[idx + 3]) {

        // TODO: Look for cleaner implementation
        if (shouldConcatenate(tokens)) {
          tokens.emplace_back(TokenType::CONCAT);
        }

        tokens.emplace_back(TokenType::CHAR_GROUP,
                            std::make_pair(line[idx + 1], line[idx + 3]));
        idx += 5;
      } else {
        throw std::runtime_error("Invalid character group");
      }
    } else if (std::isalnum(line[idx])) {
      std::string regexDef;
      while (idx < len && std::isalnum(line[idx])) {
        regexDef.push_back(line[idx++]);
      }

      // if the symbol has not been defined before throw an error
      if (regexDefinitions.count(regexDef) == 0) {
        throw std::runtime_error("Symbol \"" + regexDef +
                                 "\" has not been defined before");
      }

      const auto& regexDefTokens = regexDefinitions[regexDef];

      // TODO: Look for cleaner implementation
      if (shouldConcatenate(tokens)) {
        tokens.emplace_back(TokenType::CONCAT);
      }

      tokens.emplace_back(TokenType::LEFT_P);
      tokens.insert(tokens.end(), regexDefTokens.begin(), regexDefTokens.end());
      tokens.emplace_back(TokenType::RIGHT_P);
    } else if (line[idx] == '\\') {
      if (idx + 1 < len) {
        // TODO: Look for cleaner implementation
        if (shouldConcatenate(tokens)) {
          tokens.emplace_back(TokenType::CONCAT);
        }

        tokens.emplace_back(TokenType::RES_CHAR, line[idx + 1]);
        idx += 2;
      } else {
        throw std::runtime_error("Expected character after backslash");
      }
    }

    else {
      throw std::runtime_error("Unexpected character " +
                               std::string(1, line[idx]));
    }
  }

  regexDefinitions[identifier] = std::move(tokens);
}

void LanguageRulesParser::parseDefOrExp(std::string_view line) {
  int idx = 0;
  std::string identifier;
  while (idx < line.size() && isalpha(line[idx])) {
    identifier.push_back(line[idx++]);
  }

  if (idx == line.size() || (line[idx] != '=' && line[idx] != ':')) {
    throw std::runtime_error("Expected a definition or an expression");
  }

  if (line[idx] == '=') {
    parseDef(identifier, line, idx + 1);
  } else {
    throw std::runtime_error("Not Implemented");
  }
}

void LanguageRulesParser::parseLine(std::string_view line) {
  if (std::isalpha(line[0])) {
    parseDefOrExp(line);
  } else if (line[0] == '[') {
    throw std::runtime_error("Not Implemented");
  } else if (line[0] == '{') {
    throw std::runtime_error("Not Implemented");
  }
}

void LanguageRulesParser::printToken(const Token& token) const {
  switch (token.type) {
    case TokenType::ASTERIK: {
      std::cout << "ASTERIK";
      break;
    }
    case TokenType::OR: {
      std::cout << "OR";
      break;
    }
    case TokenType::PLUS: {
      std::cout << "PLUS";
      break;
    }
    case TokenType::CONCAT: {
      std::cout << "CONCAT";
      break;
    }
    case TokenType::DOT: {
      std::cout << "DOT";
      break;
    }
    case TokenType::LEFT_P: {
      std::cout << "LEFT_P";
      break;
    }
    case TokenType::RIGHT_P: {
      std::cout << "RIGHT_P";
      break;
    }
    case TokenType::CHAR_GROUP: {
      auto [begin, end] = std::get<std::pair<char, char>>(token.data);
      std::cout << '[' << begin << '-' << end << ']';
      break;
    }
    case TokenType::RES_CHAR: {
      char c = std::get<char>(token.data);
      std::cout << '\\' << c;
      break;
    }
    default: {
      std::cout << "ID";
      break;
    }
  }
}

void LanguageRulesParser::printTokensVector(
    const std::vector<Token>& tokens) const {
  for (const auto& token : tokens) {
    printToken(token);
    std::cout << '\t';
  }
  std::cout << std::endl;
}

void LanguageRulesParser::show() const {
  for (const auto& [identifier, tokens] : regexDefinitions) {
    std::cout << "Identifier: " << identifier << std::endl << "Tokens: ";
    printTokensVector(tokens);
  }
}

int LanguageRulesParser::tokenPrecedence(const TokenType type) const {
  switch (type) {
    case TokenType::LEFT_P:
      return 0;
    case TokenType::OR:
      return 1;
    case TokenType::CONCAT:
      return 2;
    case TokenType::PLUS:
    case TokenType::ASTERIK:
      return 3;
    default:
      throw std::runtime_error("Unsupported type");
  }
}

bool LanguageRulesParser::compareTokensByType(const Token& a,
                                              const Token& b) const {
  return tokenPrecedence(a.type) <= tokenPrecedence(b.type);
}

std::vector<Token> LanguageRulesParser::infixToPostfix(
    const std::vector<Token>& tokens) const {
  std::stack<Token> s;
  std::vector<Token> postfix;

  for (const auto& token : tokens) {
    if (token.type == TokenType::CHAR_GROUP ||
        token.type == TokenType::RES_CHAR || token.type == TokenType::DOT) {
      postfix.emplace_back(token);
    } else if (token.type == TokenType::LEFT_P) {
      s.push(token);
    } else if (token.type == TokenType::RIGHT_P) {
      while (s.top().type != TokenType::LEFT_P) {
        postfix.emplace_back(s.top());
        s.pop();
      }
      s.pop();
    } else {
      while (!s.empty() && compareTokensByType(token, s.top())) {
        postfix.emplace_back(s.top());
        s.pop();
      }
      s.push(token);
    }
  }

  while (!s.empty()) {
    postfix.emplace_back(s.top());
    s.pop();
  }
  return postfix;
}
