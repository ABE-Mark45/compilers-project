#pragma once
#include <LanguageTokens/Token.h>
#include <NFA/State.h>
#include <memory>
#include <optional>
#include <string>

// A class to represent Non deterministic finite automata. Each NFA contains a single start state and a single end state.
class NFA {
  explicit NFA(std::optional<std::string> acceptValue = std::nullopt);

  // Construct and NFA from a character group token
  // (i.e. [A-Z] creates NFA that has two states with transitions going from A to Z between them)
  static std::unique_ptr<NFA> constructCharacterGroupNFA(const Token& token);

  // consume another NFA by concatenating it (i.e. (aa)(bb) -> (aabb))
  void concatenate(std::shared_ptr<NFA> otherNFA);

  // consume another NFA and construct a combined machine with both possible paths
  // (i.e. M1: aa, M2: bb -> M1: (aa)|(bb))
  void combine(std::shared_ptr<NFA> otherNFA);

  // Applies Kleene star operation on the machine
  void kleeneStar();

  // Applies positive Kleene star operation on the machine
  void positiveKleeneStar();

 private:
  std::shared_ptr<State> startState_{nullptr};
  std::shared_ptr<State> endState_{nullptr};
};