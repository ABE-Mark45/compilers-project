#include <NFA/State.h>

namespace {
constexpr auto kLambdaTransition = '\0';
}

std::set<int> State::epsilonClosure(
    std::unordered_map<int, std::shared_ptr<State>>& idToState) const {
  std::set<int> closure;

  epsilonClosure(id_, closure, idToState);

  return closure;
}

std::set<int> State::moveThrough(
    char transition,
    std::unordered_map<int, std::shared_ptr<State>>& idToState) const {
  std::set<int> closure;

  if (transitions_.count(transition) == 0) {
    return closure;
  }

  for (int otherState : transitions_.at(transition)) {
    if (closure.count(otherState) == 0) {
      epsilonClosure(otherState, closure, idToState);
    }
  }

  return closure;
}

void State::epsilonClosure(
    int id, std::set<int>& closure,
    std::unordered_map<int, std::shared_ptr<State>>& idToState) const {
  closure.insert(id);

  const auto state = idToState.at(id);

  if (state->transitions_.count(kLambdaTransition) == 0) {
    return;
  }

  for (int otherStateId : state->transitions_.at(kLambdaTransition)) {
    if (closure.count(otherStateId) == 0) {
      epsilonClosure(otherStateId, closure, idToState);
    }
  }
}
