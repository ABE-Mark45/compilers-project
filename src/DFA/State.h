#pragma once

#include <LanguageTokens/Token.h>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

class State {
 public:
  explicit State(std::optional<std::string> acceptValue = std::nullopt)
      : acceptValue_(acceptValue) {}

  static std::unique_ptr<State> createState(
      std::optional<std::string> acceptValue = std::nullopt);

  // moves the DFA through @transition
  std::shared_ptr<State> moveThrough(char transition) const;

  // Adds a transition through character to state
  void addTransition(char transition, std::shared_ptr<const State> otherState);


  // mapping between transitions and next states
  std::unordered_map<char, std::shared_ptr<const State>>
      transitions_;
  // Which language token this state accepts
  std::optional<std::string> acceptValue_;
};
