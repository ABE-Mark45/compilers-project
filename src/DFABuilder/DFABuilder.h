#pragma once
#include <DFA/State.h>
#include <NFA/NFA.h>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <unordered_set>

namespace DFABuilder {
std::shared_ptr<const dfa::State> buildDFA(std::unique_ptr<nfa::NFA> nfa);
std::shared_ptr<const dfa::State> minimizeDFA(std::shared_ptr<const dfa::State> startingState);
std::vector<std::set<std::shared_ptr<const dfa::State>>> getAcceptingAndRejectingStates(std::shared_ptr<const dfa::State> startingState);

};  // namespace DFABuilder