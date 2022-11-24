#include "LanguageRulesParser/LanguageRulesParser.h"

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace {
std::unordered_set<char> regexCharacters{'*', '|', '+', '.', '(', ')'};
};

using namespace Token;

void LanguageRulesParser::parseDef(const std::string& identifier,
                                   const std::string& line, int idx) {
  int len = line.size();
  std::vector<TokenType> tokens;

  while (idx < len) {
    if (regexCharacters.count(line[idx])) {
      tokens.emplace_back(MetaCharacter(line[idx]));
      idx++;
    } else if (line[idx] == '[') {
      if (idx + 4 < len && std::isalnum(line[idx + 1]) &&
          line[idx + 2] == '-' && std::isalnum(line[idx + 3]) &&
          line[idx + 4] == ']' && line[idx + 1] <= line[idx + 3]) {
        tokens.emplace_back(CharacterGroup(line[idx + 1], line[idx + 3]));
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
      tokens.emplace_back(MetaCharacter('('));
      tokens.insert(tokens.end(), regexDefTokens.begin(), regexDefTokens.end());
      tokens.emplace_back(MetaCharacter(')'));
    } else if (line[idx] == '\\') {
      if (idx + 1 < len) {
        tokens.emplace_back(ReservedCharacter(line[idx + 1]));
        idx += 2;
      } else {
        throw std::runtime_error("Expected character after backslash");
      }
    }

    else {
      throw std::runtime_error("Unexpected character " + line[idx]);
    }
  }

  regexDefinitions[identifier] = tokens;
}

void LanguageRulesParser::parseDefOrExp(const std::string& line) {
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

void LanguageRulesParser::parseLine(const std::string& line) {
  if (std::isalpha(line[0])) {
    parseDefOrExp(line);
  } else if (line[0] == '[') {
    throw std::runtime_error("Not Implemented");
  } else if (line[0] == '{') {
    throw std::runtime_error("Not Implemented");
  }
}

void LanguageRulesParser::show() {
  for (const auto& [identifier, tokens] : regexDefinitions) {
    std::cout << "Identifier: " << identifier << std::endl << "Tokens: ";
    for (const auto& token : tokens) {
      if (auto* t = std::get_if<MetaCharacter>(&token)) {
        std::cout << t->c_;
      } else if (auto* t = std::get_if<Identifier>(&token)) {
        std::cout << t->name_;
      } else if (auto* t = std::get_if<ReservedCharacter>(&token)) {
        std::cout << '\\' << t->c_;
      } else if (auto* t = std::get_if<CharacterGroup>(&token)) {
        std::cout << t->begin_ << '-' << t->end_;
      }

      std::cout << '\t';
    }
    std::cout << std::endl;
  }
}
