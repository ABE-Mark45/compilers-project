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
      : id_(sStatesCount++), acceptValue_(acceptValue) {}

  void setAcceptValue(const std::string& acceptValue) {
    acceptValue_ = acceptValue;
  }

  static std::unique_ptr<State> createState(
      std::optional<std::string> acceptValue = std::nullopt);

  // Computes epsilon closure of the current state given the mapping between each state ID and its internal data
  std::set<std::shared_ptr<const State>> epsilonClosure() const;

  // Computes epsilon closure of the current state after moving through
  // the specified transition given the mapping between each state ID and its internal data
  std::set<std::shared_ptr<const State>> moveThrough(char transition) const;

  // Adds a transition through character to state
  void addTransition(char transition, std::shared_ptr<const State> otherState);

  // A convenience function for epsilon transitions
  void addEpsilonTransition(std::shared_ptr<const State> otherState);

 private:
  // A utility function for the epsilon closure
  void epsilonClosure(std::shared_ptr<const State> state,
                      std::set<std::shared_ptr<const State>>& closure) const;

  // State ID
  int id_;
  // mapping between transitions and states
  std::unordered_map<char, std::vector<std::shared_ptr<const State>>>
      transitions_;
  // State accept value
  std::optional<std::string> acceptValue_;
  // id generator
  static inline int sStatesCount{0};
};