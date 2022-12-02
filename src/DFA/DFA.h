#pragma once
#include <LanguageTokens/Token.h>
#include <DFA/State.h>
#include <NFA/NFA.h>
#include <memory>
#include <optional>
#include <string>

// A class to represent DFAs. A DFA has a single start state and multiple end states
class DFA {
  // the DFA will accept multiple language tokens. Create a DFA from NFA via subset construction.
  explicit DFA(std::shared_ptr<NFA::NFA> NFA);

  std::unique_ptr<DFA> minimize();

  // consume the longest prefix from @input starting from index @ptr, and advance @ptr accordingly.
  // @returns the language token type.
  std::string consume(int *ptr, std::string input);

 private:
  std::shared_ptr<State> startState_{nullptr};
};