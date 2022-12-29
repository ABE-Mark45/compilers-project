#include "LanguageRulesParser/LanguageRulesParser.h"

#include <iostream>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace {
std::unordered_set<char> regexCharacters{'*', '|', '+', '(', ')'};
};

bool LanguageRulesParser::shouldConcatenate(
    const std::vector<Token>& tokens) const {
  if (tokens.empty()) {
    return false;
  }

  const auto& token = tokens.back();

  switch (token.type) {
    case TokenType::CHAR_GROUP:
    case TokenType::CHAR:
    case TokenType::RIGHT_P:
    case TokenType::ASTERIK:
    case TokenType::PLUS:
    case TokenType::EPSILON:
      return true;
    default:
      return false;
  }
}

std::vector<Token> LanguageRulesParser::parseDef(std::string_view line,
                                                 int idx) {
  int len = line.size();
  std::vector<Token> tokens;

  while (idx < len) {
    if (std::isspace(line[idx])) {
      idx++;
      continue;
    } else if (regexCharacters.count(line[idx])) {  // If special character
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
        case '|':
          tokens.emplace_back(TokenType::OR);
          break;
        default:
          throw std::runtime_error("Unrecognized symbol");
      }

      idx++;
    } else if (line[idx] == '[') {  // If character group
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
    } else if (std::isalnum(line[idx])) {  // If regex definition
      std::string regexDef;
      while (idx < len && std::isalnum(line[idx])) {
        regexDef.push_back(line[idx++]);
      }

      // if the symbol has not been defined, it means this is a normal text
      if (regexDefinitions_.count(regexDef) == 0) {
        for (char c : regexDef) {
          if (shouldConcatenate(tokens)) {
            tokens.emplace_back(TokenType::CONCAT);
          }
          tokens.emplace_back(TokenType::CHAR, c);
        }
      } else {

        const auto& regexDefTokens = regexDefinitions_[regexDef];

        // TODO: Look for cleaner implementation
        if (shouldConcatenate(tokens)) {
          tokens.emplace_back(TokenType::CONCAT);
        }

        tokens.emplace_back(TokenType::LEFT_P);
        tokens.insert(tokens.end(), regexDefTokens.begin(),
                      regexDefTokens.end());
        tokens.emplace_back(TokenType::RIGHT_P);
      }
    } else if (line[idx] == '\\') {  // if reserved character
      if (idx + 1 < len) {
        // TODO: Look for cleaner implementation
        if (shouldConcatenate(tokens)) {
          tokens.emplace_back(TokenType::CONCAT);
        }

        if (line[idx + 1] == 'L') {
          tokens.emplace_back(TokenType::EPSILON, '\0');
        } else {
          tokens.emplace_back(TokenType::CHAR, line[idx + 1]);
        }
        idx += 2;
      } else {
        throw std::runtime_error("Expected character after backslash");
      }
    } else {  // punctuation character
      if (shouldConcatenate(tokens)) {
        tokens.emplace_back(TokenType::CONCAT);
      }
      tokens.emplace_back(TokenType::CHAR, line[idx]);
      idx++;
    }
  }

  return tokens;
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

  // TODO: Change name
  auto tokens = parseDef(line, idx + 1);

  if (line[idx] == ':') {
    postfixRegexExpressions_[identifier] = infixToPostfix(tokens);
    regexExpressions_[identifier] = std::move(tokens);
    priorities_[identifier] = priorities_.size() + 2;
  } else {
    regexDefinitions_[identifier] = std::move(tokens);
  }
}

void LanguageRulesParser::parseKeywords(std::string_view line) {
  std::stringstream ss{std::string{line}};
  std::string keyword;

  while (std::getline(ss, keyword, ' ')) {
    if (keyword.length() > 0) {
      keywords_.emplace_back(keyword);
      priorities_[keyword] = 0;
    }
  }
}

void LanguageRulesParser::parsePunctuation(std::string_view line) {
  std::stringstream ss{std::string{line}};
  std::string punctuation;

  while (std::getline(ss, punctuation, ' ')) {
    if (punctuation.empty()) {
      continue;
    } else if (punctuation.length() == 1) {
      punctuationCharacters_.emplace_back(punctuation[0]);
      priorities_[std::string(1, punctuation[0])] = 1;
    } else if (punctuation.length() == 2 && punctuation[0] == '\\') {
      punctuationCharacters_.emplace_back(punctuation[1]);
      priorities_[std::string(1, punctuation[1])] = 1;
    } else {
      throw std::runtime_error("Invalid punctuation");
    }
  }
}

void LanguageRulesParser::parseLine(std::string_view line) {
  if (std::isalpha(line[0])) {
    parseDefOrExp(line);
  } else if (line[0] == '[') {
    if (line.back() != ']') {
      throw std::runtime_error("Invalid punctuation characters line");
    }
    // Parse line without [] characters
    parsePunctuation(line.substr(1, line.length() - 2));
  } else if (line[0] == '{') {
    if (line.back() != '}') {
      throw std::runtime_error("Invalid keywords line");
    }
    // Parse line without {} characters
    parseKeywords(line.substr(1, line.length() - 2));
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
    case TokenType::EPSILON: {
      std::cout << "EPSILON";
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
    case TokenType::CHAR: {
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
  for (const auto& [identifier, tokens] : regexDefinitions_) {
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
    if (token.type == TokenType::CHAR_GROUP || token.type == TokenType::CHAR ||
        token.type == TokenType::EPSILON) {
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