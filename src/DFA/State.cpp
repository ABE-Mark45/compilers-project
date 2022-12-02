#include <NFA/State.h>

namespace {
constexpr auto kEpsilonTransition = '\0';
}

/*static*/ std::unique_ptr<State> createState(
    std::optional<std::string> acceptValue) {
  return std::make_unique<State>(acceptValue);
}

std::shared_ptr<State> State::moveThrough(char transition) const {
  return transitions_.at(transition);
}


void State::addTransition(char transition,
                          std::shared_ptr<const State> otherState) {
  transitions_[transition] = otherState;
}
