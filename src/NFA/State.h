#pragma once

#include <LanguageTokens/Token.h>
#include <utils/AcceptValue.h>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace nfa {

class State : public std::enable_shared_from_this<State> {
 public:
  explicit State(std::optional<AcceptValue> acceptValue = std::nullopt)
      : id_(sStatesCount++), acceptValue_(acceptValue) {}

  std::optional<AcceptValue> getAcceptValue() const { return acceptValue_; }

  void setAcceptValue(const AcceptValue& acceptValue) {
    acceptValue_ = acceptValue;
  }

  // Computes epsilon closure of the current state given the mapping between each state ID and its internal data
  std::pair<std::set<std::shared_ptr<const State>>, std::optional<AcceptValue>>
  epsilonClosure() const;

  // Computes epsilon closure of the current state after moving through
  // the specified transition given the mapping between each state ID and its internal data
  std::pair<std::set<std::shared_ptr<const State>>, std::optional<AcceptValue>>
  moveThrough(char transition) const;

  // Adds a transition through character to state
  void addTransition(char transition, std::shared_ptr<const State> otherState);

  // A convenience function for epsilon transitions
  void addEpsilonTransition(std::shared_ptr<const State> otherState);

 private:
  // A utility function for the epsilon closure
  static void epsilonClosure(std::shared_ptr<const State> state,
                             std::set<std::shared_ptr<const State>>& closure,
                             AcceptValue& acceptValue);

  // State ID
  int id_;
  // mapping between transitions and states
  std::unordered_map<char, std::vector<std::shared_ptr<const State>>>
      transitions_;
  // State accept value
  std::optional<AcceptValue> acceptValue_;
  // id generator
  static inline int sStatesCount{0};
};
}  // namespace nfa