#pragma once
#include <NFA/NFA.h>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <unordered_set>

namespace DFABuilder {
std::shared_ptr<const State> buildDFA(std::unique_ptr<NFA> nfa);

};  // namespace DFABuilder