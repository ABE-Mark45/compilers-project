#pragma once
#include <string>
#include <variant>

namespace Token {
struct CharacterGroup {
  CharacterGroup(char begin, char end) : begin_(begin), end_(end) {}
  CharacterGroup() = delete;
  bool operator==(const CharacterGroup& o) const {
    return this->begin_ == o.begin_ && this->end_ == o.end_;
  }

  char begin_;
  char end_;
};

struct MetaCharacter {
  MetaCharacter(char c) : c_(c) {}
  MetaCharacter() = delete;
  bool operator==(const MetaCharacter& o) const { return this->c_ == o.c_; }

  char c_;
};

struct Identifier {
  Identifier(const std::string& name) : name_(name) {}
  Identifier() = delete;
  bool operator==(const Identifier& o) const { return this->name_ == o.name_; }
  std::string name_;
};

struct ReservedCharacter {
  ReservedCharacter(char c) : c_(c) {}
  ReservedCharacter() = delete;
  bool operator==(const ReservedCharacter& o) const { return this->c_ == o.c_; }

  char c_;
};
}  // namespace Token