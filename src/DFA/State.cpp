#include <DFA/State.h>

namespace {
constexpr auto kEpsilonTransition = '\0';
}

namespace dfa {

std::shared_ptr<const State> State::moveThrough(char transition) const {
  if (transitions_.count(transition) == 0) {
    return nullptr;
  }
  return transitions_.at(transition);
}

void State::addTransition(char transition,
                          std::shared_ptr<const State> otherState) {
  if (transitions_.count(transition)) {
    throw std::runtime_error("An existing transition already exists");
  }
  transitions_[transition] = otherState;
}
}  // namespace dfa