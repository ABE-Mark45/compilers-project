#include <NFA/State.h>

namespace {
constexpr auto kEpsilonTransition = '\0';
}

namespace nfa {
std::pair<std::set<std::shared_ptr<const State>>, std::optional<AcceptValue>>
State::epsilonClosure() const {
  std::set<std::shared_ptr<const State>> closure;
  AcceptValue acceptValue;

  epsilonClosure(shared_from_this(), closure, acceptValue);

  if (acceptValue.priority == INT32_MAX) {
    return {closure, std::nullopt};
  }
  return {closure, acceptValue};
}

std::pair<std::set<std::shared_ptr<const State>>, std::optional<AcceptValue>>
State::moveThrough(char transition) const {
  std::set<std::shared_ptr<const State>> closure;

  if (transitions_.count(transition) == 0) {
    return {closure, std::nullopt};
  }

  AcceptValue acceptValue;
  for (const auto otherState : transitions_.at(transition)) {
    if (closure.count(otherState) == 0) {
      epsilonClosure(otherState, closure, acceptValue);
    }
  }
  if (acceptValue.priority == INT32_MAX) {
    return {closure, std::nullopt};
  }
  return {closure, acceptValue};
}

/*static*/ void State::epsilonClosure(
    std::shared_ptr<const State> state,
    std::set<std::shared_ptr<const State>>& closure, AcceptValue& acceptValue) {
  closure.insert(state);
  acceptValue.reduceMin(state->acceptValue_);
  // if no epsilon transitions from s, end the algorithm
  if (state->transitions_.count(kEpsilonTransition) == 0) {
    return;
  }

  // for each state that can be reached from s via an epsilon transition
  for (const auto otherState : state->transitions_.at(kEpsilonTransition)) {
    // if it's not already in the closure
    if (closure.count(otherState) == 0) {
      // add its epsilon closure to the set
      epsilonClosure(otherState, closure, acceptValue);
    }
  }
}

void State::addTransition(char transition,
                          std::shared_ptr<const State> otherState) {
  transitions_[transition].emplace_back(otherState);
}

void State::addEpsilonTransition(std::shared_ptr<const State> otherState) {
  addTransition(kEpsilonTransition, otherState);
}

}  // namespace nfa