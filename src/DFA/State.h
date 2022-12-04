#pragma once

#include <utils/AcceptValue.h>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace dfa {
class State {
 public:
  explicit State(std::optional<AcceptValue> acceptValue = std::nullopt)
      : id_(sStatesCount++), acceptValue_(acceptValue) {}

  std::optional<AcceptValue> getAcceptValue() const { return acceptValue_; }

  int getId() const { return id_; }

  void setAcceptValue(const AcceptValue& acceptValue) {
    acceptValue_ = acceptValue;
  }

  // moves the DFA through @transition
  std::shared_ptr<const State> moveThrough(char transition) const;

  // Adds a transition through character to state
  void addTransition(char transition, std::shared_ptr<const State> otherState);

 private:
  int id_;
  // mapping between transitions and next states
  std::unordered_map<char, std::shared_ptr<const State>> transitions_;
  // Which language token this state accepts
  std::optional<AcceptValue> acceptValue_;

  static inline int sStatesCount{0};
};
}  // namespace dfa