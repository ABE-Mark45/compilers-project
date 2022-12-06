#include <DFABuilder/DFABuilder.h>
#include <NFA/State.h>
#include <utils/AcceptValue.h>
#include <algorithm>
#include <iostream>
#include <queue>

using namespace std;

namespace {
using ClosureType = std::set<std::shared_ptr<const nfa::State>>;
using VisitRetunType = std::pair<ClosureType, std::optional<AcceptValue>>;
using MembershipTableType =
    std::unordered_map<std::shared_ptr<const dfa::State>, int>;
// A utility function to generate all the possible states which can be reached from the current closure
VisitRetunType getNewClosure(ClosureType& closure, char transition) {
  ClosureType newClosure;
  AcceptValue combinedAcceptValue;
  // for each state in the new closure, get the reacheable states and add them to the combined closure
  for (auto state : closure) {
    auto [movedToStates, acceptValue] = state->moveThrough(transition);
    combinedAcceptValue.reduceMin(acceptValue);
    newClosure.insert(movedToStates.begin(), movedToStates.end());
  }
  if (combinedAcceptValue.priority == INT32_MAX) {
    return {newClosure, std::nullopt};
  }
  // return the new closure
  return {newClosure, combinedAcceptValue};
}

std::pair<MembershipTableType, int> getAcceptingAndRejectingStates(
    std::shared_ptr<const dfa::State> startState) {

  set<shared_ptr<const dfa::State>> acceptingStates, rejectingStates;
  std::unordered_set<shared_ptr<const dfa::State>> visited{startState};
  queue<shared_ptr<const dfa::State>> q;
  MembershipTableType membership;
  unordered_map<string, int> acceptValueToIndex;

  q.push(startState);
  while (!q.empty()) {
    auto currentState = q.front();
    q.pop();

    auto currentAcceptValue = currentState->getAcceptValue();
    if (currentAcceptValue == std::nullopt) {
      membership[currentState] = 0;
    } else {
      string val = currentAcceptValue.value().value;
      if (acceptValueToIndex.count(val) == 0) {
        acceptValueToIndex[val] = acceptValueToIndex.size() + 1;
      }
      membership[currentState] = acceptValueToIndex.at(val);
    }

    for (unsigned char c = 1; c <= 127; c++) {
      auto nextState = currentState->moveThrough(c);
      if (nextState && visited.count(nextState) == 0) {
        visited.insert(nextState);
        q.push(nextState);
      }
    }
  }

  return {membership, acceptValueToIndex.size() + 1};
}

auto isStatesGroupEquivalent =
    [](shared_ptr<const dfa::State> a, shared_ptr<const dfa::State> b,
       const MembershipTableType& oldMembershipTable) {
      auto acceptValueA = a->getAcceptValue();
      auto acceptValueB = b->getAcceptValue();
      if ((acceptValueA.has_value() && !acceptValueB.has_value()) ||
          (!acceptValueA.has_value() && acceptValueB.has_value()) ||
          (acceptValueA.has_value() && acceptValueB.has_value() &&
           acceptValueA.value().value != acceptValueB.value().value)) {
        return false;
      }

      for (unsigned char c = 1; c <= 127; c++) {
        auto nextStateA = a->moveThrough(c);
        auto nextStateB = b->moveThrough(c);
        // check if they are in the same set

        if (nextStateA == nullptr && nextStateB == nullptr) {
          continue;
        } else if (nextStateA == nullptr || nextStateB == nullptr) {
          return false;
        } else if (oldMembershipTable.at(nextStateA) !=
                   oldMembershipTable.at(nextStateB))
          return false;
      }
      return true;
    };

auto getReachableStates = [](std::shared_ptr<const dfa::State> startState) {
  std::unordered_set<std::shared_ptr<const dfa::State>> reachable;
  queue<std::shared_ptr<const dfa::State>> q;

  reachable.insert(startState);
  q.push(startState);

  while (!q.empty()) {
    auto state = q.front();
    q.pop();

    for (int i = 1; i <= 127; i++) {
      if (auto nextState = state->moveThrough(i)) {
        if (reachable.count(nextState) == 0) {
          reachable.insert(nextState);
          q.push(nextState);
        }
      }
    }
  }
  return reachable;
};

}  // namespace

namespace DFABuilder {
std::shared_ptr<const dfa::State> buildDFA(std::unique_ptr<nfa::NFA> nfa) {
  // Get the start state of the NFA
  auto startState = nfa->getStartState();
  // A mapping between closure and the corresponding states in the DFA.
  // For each closure generated from the NFA, a corresponding state is created in the DFA
  std::map<ClosureType, std::shared_ptr<dfa::State>> closureToDFAStatesMapping;
  // Closure of the start state of the NFA
  auto [startClosure, initialAcceptValue] = startState->epsilonClosure();

  // Queue for the closures to be processed later as part of the BFS algorithm
  std::queue<ClosureType> closures;
  // Push the start closure to be processed
  closures.push(startClosure);
  // create the initial state of the DFA
  closureToDFAStatesMapping[startClosure] =
      std::make_shared<dfa::State>(initialAcceptValue);

  while (!closures.empty()) {
    // Get the closure to be processed
    auto closure = closures.front();
    closures.pop();
    // Get the DFA state corresponding to that closure
    auto currentState = closureToDFAStatesMapping[closure];

    // For each printable character
    for (unsigned char c = 1; c <= 127; c++) {
      // Get all the states and their epsilon closures which can be visted from the current states in the closure
      auto [newClosure, newAcceptValue] = getNewClosure(closure, c);
      // If this yields no states, continue
      if (newClosure.empty()) {
        continue;
      }

      // If this is the first time to encounter this closure, create a new state corresponding to it
      // and add it to the queue of closures to be processed
      if (closureToDFAStatesMapping.count(newClosure) == 0) {
        closureToDFAStatesMapping[newClosure] =
            std::make_shared<dfa::State>(newAcceptValue);
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

std::shared_ptr<const dfa::State> minimizeDFA(
    std::shared_ptr<const dfa::State> startState) {
  // construct an initial set of accepting and rejecting states

  auto [newGroupMembership, newGroupsCount] =
      getAcceptingAndRejectingStates(startState);
  MembershipTableType oldGroupMembership;

  int oldGroupsCount;
  std::vector<unordered_set<shared_ptr<const dfa::State>>> groups;

  do {
    oldGroupMembership.swap(newGroupMembership);
    oldGroupsCount = newGroupsCount;

    newGroupMembership.clear();
    groups.clear();

    for (const auto [currentState, currentMembership] : oldGroupMembership) {
      bool isCurrentStateAddedToGroup = false;

      for (int groupIdx = 0; groupIdx < groups.size(); groupIdx++) {
        auto& group = groups[groupIdx];
        const auto groupState = *group.begin();

        if (isStatesGroupEquivalent(currentState, groupState,
                                    oldGroupMembership)) {
          isCurrentStateAddedToGroup = true;
          group.insert(currentState);
          newGroupMembership[currentState] = groupIdx;
          break;
        }
      }

      if (!isCurrentStateAddedToGroup) {
        groups.push_back({currentState});
        newGroupMembership[currentState] = groups.size() - 1;
      }
    }

    newGroupsCount = groups.size();
  } while (oldGroupsCount != newGroupsCount);

  vector<shared_ptr<dfa::State>> resultStates;
  shared_ptr<dfa::State> resultStartingState;

  dfa::State::resetStatesCount();
  for (const auto& group : groups) {
    auto groupAcceptValue = (*group.begin())->getAcceptValue();
    auto repState = make_shared<dfa::State>(groupAcceptValue);
    resultStates.emplace_back(repState);

    if (group.count(startState) == 1) {
      resultStartingState = repState;
    }
  }

  // construct transitions
  for (int i = 0; i < resultStates.size(); i++) {
    // cout << i << endl;
    auto resultState = resultStates[i];
    auto repState = *groups[i].begin();
    for (unsigned char c = 1; c <= 127; c++) {
      if (auto nextState = repState->moveThrough(c)) {
        int nextStateMembership = newGroupMembership.at(nextState);
        resultState->addTransition(c, resultStates[nextStateMembership]);
      }
    }
  }

  auto reachableStates = getReachableStates(resultStartingState);

  std::for_each(resultStates.begin(), resultStates.end(),
                [&reachableStates, idx = 0](auto& state) mutable -> void {
                  if (reachableStates.count(state)) {
                    state->setId(idx++);
                  }
                });

  return resultStartingState;
}

}  // namespace DFABuilder