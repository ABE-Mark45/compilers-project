#include <NFA/State.h>

namespace {
constexpr auto kEpsilonTransition = '\0';
}

/*static*/ std::unique_ptr<State> createState(
    std::optional<std::string> acceptValue) {
  return std::make_unique<State>(acceptValue);
}

std::set<int> State::epsilonClosure() const {
  std::set<int> closure;

  epsilonClosure(std::make_shared<const State>(this), closure);
  return closure;
}

std::set<int> State::moveThrough(char transition) const {
  std::set<int> closure;

  if (transitions_.count(transition) == 0) {
    return closure;
  }

  for (const auto otherState : transitions_.at(transition)) {
    if (closure.count(otherState->id_) == 0) {
      epsilonClosure(otherState, closure);
    }
  }

  return closure;
}

void State::epsilonClosure(std::shared_ptr<const State> s,
                           std::set<int>& closure) const {
  closure.insert(state->id_);

  // if no epsilon transitions from s, end the algorithm
  if (s->transitions_.count(kEpsilonTransition) == 0) {
    return;
  }

  // for each state that can be reached from s via an epsilon transition
  for (const auto otherState : s->transitions_.at(kEpsilonTransition)) {
    // if it's not already in the closure
    if (closure.count(otherState->id_) == 0) {
      // add its epsilon closure to the set
      epsilonClosure(otherState, closure);
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