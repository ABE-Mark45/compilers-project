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
  struct AcceptValue {
    int priority;
    std::string value;

    AcceptValue(int priority, const std::string& value)
        : priority(priority), value(value) {}
    AcceptValue(const AcceptValue& other)
        : priority(other.priority), value(other.value) {}

    AcceptValue() : priority(INT32_MAX) {}

    bool operator<(const AcceptValue& other) const {
      return priority < other.priority;
    };

    void reduceMin(std::optional<AcceptValue> other) {
      if (other && other->priority < priority) {
        priority = other->priority;
        value = other->value;
      }
    }
  };

  explicit State(std::optional<AcceptValue> acceptValue = std::nullopt)
      : id_(sStatesCount++), acceptValue_(acceptValue) {}

  void setAcceptValue(const AcceptValue& acceptValue) {
    acceptValue_ = acceptValue;
  }

  static std::unique_ptr<State> createState(
      std::optional<std::string> acceptValue = std::nullopt);

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