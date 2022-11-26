#pragma once

#include <LanguageTokens/Token.h>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

class State {

  explicit State(std::optional<std::string> acceptValue = std::nullopt)
      : acceptValue_(acceptValue) {}

  static std::unique_ptr<State> createState(
      std::optional<std::string> acceptValue = std::nullopt);

  // Computes epsilon closure of the current state given the mapping between each state ID and its internal data
  std::set<int> epsilonClosure() const;

  // Computes epsilon closure of the current state after moving through
  // the specified transition given the mapping between each state ID and its internal data
  std::set<int> moveThrough(char transition) const;

  // Adds a transition through character to state
  void addTransition(char transition, std::shared_ptr<State> otherState);

  // A convenience function for epsilon transitions
  void addLambdaTransition(std::shared_ptr<State> otherState);

 private:
  // A utility function for the epsilon closure
  void epsilonClosure(std::shared_ptr<State> state,
                      std::set<int>& closure) const;

  // State ID
  int id_;
  // mapping between transitions and states
  std::unordered_map<char, std::vector<std::shared_ptr<State>>> transitions_;
  // State accept value
  std::optional<std::string> acceptValue_;
};
