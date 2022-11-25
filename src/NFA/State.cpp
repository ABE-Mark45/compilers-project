#include <NFA/State.h>

namespace {
constexpr auto kLambdaTransition = '\0';
}

std::set<int> State::epsilonClosure() const {
  std::set<int> closure;

  epsilonClosure(std::make_shared<State>(this), closure);
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

void State::epsilonClosure(std::shared_ptr<State> state,
                           std::set<int>& closure) const {
  closure.insert(state->id_);

  if (state->transitions_.count(kLambdaTransition) == 0) {
    return;
  }

  for (const auto otherState : state->transitions_.at(kLambdaTransition)) {
    if (closure.count(otherState->id_) == 0) {
      epsilonClosure(otherState, closure);
    }
  }
}
