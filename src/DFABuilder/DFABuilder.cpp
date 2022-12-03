#include <DFABuilder/DFABuilder.h>

namespace {
using ClosureType = std::set<std::shared_ptr<const State>>;
// A utility function to generate all the possible states which can be reached from the current closure
ClosureType getNewClosure(ClosureType& closure, char transition) {
  ClosureType newClosure;
  // for each state in the new closure, get the reacheable states and add them to the combined closure
  for (auto state : closure) {
    auto movedToStates = state->moveThrough(transition);
    newClosure.insert(movedToStates.begin(), movedToStates.end());
  }
  // return the new closure
  return newClosure;
}
}  // namespace

namespace DFABuilder {
std::shared_ptr<const State> buildDFA(std::unique_ptr<NFA> nfa) {
  // Get the start state of the NFA
  auto startState = nfa->getStartState();
  // A mapping between closure and the corresponding states in the DFA.
  // For each closure generated from the NFA, a corresponding state is created in the DFA
  std::map<ClosureType, std::shared_ptr<State>> closureToDFAStatesMapping;
  // Closure of the start state of the NFA
  auto startClosure = startState->epsilonClosure();

  // Queue for the closures to be processed later as part of the BFS algorithm
  std::queue<ClosureType> closures;
  // Push the start closure to be processed
  closures.push(startClosure);
  // create the initial state of the DFA
  closureToDFAStatesMapping[startClosure] = std::make_shared<State>();

  while (!closures.empty()) {
    // Get the closure to be processed
    auto closure = closures.front();
    closures.pop();
    // Get the DFA state corresponding to that closure
    auto currentState = closureToDFAStatesMapping[closure];

    // For each printable character
    for (char c = 1; c <= 127; c++) {
      // Get all the states and their epsilon closures which can be visted from the current states in the closure
      auto newClosure = getNewClosure(closure, c);
      // If this yields no states, continue
      if (newClosure.empty()) {
        continue;
      }

      // If this is the first time to encounter this closure, create a new state corresponding to it
      // and add it to the queue of closures to be processed
      if (closureToDFAStatesMapping.count(newClosure) == 0) {
        closureToDFAStatesMapping[newClosure] = std::make_shared<State>();
        closures.push(newClosure);
      }

      // Get the state corresponding to the new closure
      auto toState = closureToDFAStatesMapping[newClosure];
      // Add a transition between the current state and the other state with the value of character 'c'
      currentState->addTransition(c, toState);
    }
  }
  // return the initial state of the DFA
  return closureToDFAStatesMapping[startClosure];
}
}  // namespace DFABuilder