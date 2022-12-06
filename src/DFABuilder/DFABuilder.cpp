#include <DFABuilder/DFABuilder.h>
#include <NFA/State.h>
#include <utils/AcceptValue.h>
#include <queue>
#include <iostream>

using namespace std;

namespace {
using ClosureType = std::set<std::shared_ptr<const nfa::State>>;
using VisitRetunType = std::pair<ClosureType, std::optional<AcceptValue>>;
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

void print(auto& oldEqv) {
    cout << oldEqv.size() << "\n";
    for(auto& set: oldEqv) {
      cout <<"(";
      for(auto& state: set) {
        cout << state->getId() << ",";
      }
      cout << ") - ";
    } cout << "\n";
}
  
std::shared_ptr<const dfa::State> minimizeDFA(std::shared_ptr<const dfa::State> startingState){

  // construct an initial set of accepting and rejecting states
  
  vector<set<shared_ptr<const dfa::State>>> newEqv = getAcceptingAndRejectingStates(startingState);
  vector<set<shared_ptr<const dfa::State>>> oldEqv;

  auto findStateIndex = [](shared_ptr<const dfa::State> state, vector<set<shared_ptr<const dfa::State>>>& eqv){
    
    for(int j=0; j<eqv.size(); j++){
        auto& set = eqv[j];
        if(set.count(state)) {
          return j;
        }
    }
    return -1;
  };

  auto is_equivalent = [&](shared_ptr<const dfa::State> a,  shared_ptr<const dfa::State> b, vector<set<shared_ptr<const dfa::State>>>& eqv){  

    if(findStateIndex(a, eqv) != findStateIndex(b, eqv))
      return false;

    if(a->getAcceptValue().has_value() && b->getAcceptValue().has_value()){
      if(a->getAcceptValue().value().value != b->getAcceptValue().value().value)
        return false;
    }

    for (unsigned char c = 1; c <= 127; c++) {
      auto nextStateA = a->moveThrough(c);
      auto nextStateB = b->moveThrough(c);
      // check if they are in the same set

      if(findStateIndex(nextStateA, eqv) != findStateIndex(nextStateB, eqv))
        return false;
    }
    return true; 
  };

  do {
    oldEqv = newEqv; // TODO: use move
    newEqv.clear();
    
    // print(oldEqv);

    for(auto& set: oldEqv) {
      // print(oldEqv);
      // cout << "setrep: " << (*set.begin())->getId() <<"\n";
      // make a new set for the first element that reperesnts it
      shared_ptr<const dfa::State> setRep = *set.begin();
      
      std::set<shared_ptr<const dfa::State>> newSet({setRep}); 
      // add the set to the represntative sets
      std::vector<std::set<shared_ptr<const dfa::State>>> newSets;
      newSets.push_back(newSet);
      for(auto state: set) {
        // cout << "state: "<<  state->getId() << ": ";
        // assume the state to in its own set until proven otherwise
        bool isNewRep = true;
        for(auto& newSet: newSets) {
          shared_ptr<const dfa::State> newSetRep = *newSet.begin();
          // cout << newSetRep->getId() <<" ";
          if(newSetRep->getId() == state->getId()) {
            isNewRep = false;
            break;
          }

          if(is_equivalent(state, newSetRep, oldEqv)) {
            newSet.insert(state);
            isNewRep = false;
            break;
          }
        } 
        // cout << "\n";
        if(isNewRep) {
          std::set<shared_ptr<const dfa::State>> newSet{state};
          newSets.push_back(newSet);
        }
      } 
  
      newEqv.insert(newEqv.end(), newSets.begin(), newSets.end());
      // cout <<" seg\n";
    }
    // // cout <<"check\n";
    
  } while(newEqv.size() != oldEqv.size());

  vector<shared_ptr<dfa::State>> resultStates;
  shared_ptr<dfa::State> resultStartingState;
  // construct all states (without transitions)
  // cout << "construct states\n";
  for(auto& set: newEqv) {
    shared_ptr<dfa::State> repState = make_shared<dfa::State>();
    if( (*set.begin())->getAcceptValue().has_value()){
      repState->setAcceptValue((*set.begin())->getAcceptValue().value());
    }
    resultStates.push_back(repState);

    if(set.count(startingState) == 1) {
      resultStartingState = repState;
    }
  }
  // cout << "construct transitions\n";

  // construct transitions
  for(int i=0; i<resultStates.size(); i++) {
    // cout << i << endl;
    auto resultState = resultStates[i];
    auto repState = *newEqv[i].begin();
    for (unsigned char c = 1; c <= 127; c++) {
      auto nextState = repState->moveThrough(c);
      // find next state
      int ind = findStateIndex(nextState, oldEqv);
      if(ind != -1)
        resultState->addTransition(c, resultStates[ind]); 
    }
    // cout << resultState.get()->getId() <<" -> " << 
    //   resultState->moveThrough(1).get()->getId() << "," << resultState->moveThrough(2).get()->getId() << std::endl;
  }
  
  // std::cout << "size: " << resultStates.size() << std::endl; 
  return resultStartingState;
}


vector<set<shared_ptr<const dfa::State>>> getAcceptingAndRejectingStates(std::shared_ptr<const dfa::State> startingState) {
  
  set<shared_ptr<const dfa::State>> acceptingStates, rejectingStates;
  std::unordered_set<int> visited;
  queue<shared_ptr<const dfa::State>> q;
  q.push(startingState);
  
  while(!q.empty()) {
    auto state = q.front();
    q.pop();
    
    if(visited.count(state->getId()) != 0) {
      // visited before
      continue;;
    }
    visited.insert(state->getId());
    
    if (state->getAcceptValue()) {
      acceptingStates.insert(state);
    } else {
      rejectingStates.insert(state);
    }

    for (unsigned char c = 1; c <= 127; c++) {
      auto nextState = state->moveThrough(c);
      if (nextState) {
        if (visited.count(nextState->getId()) == 0) {
          q.push(nextState);
        }
      }
    } 

  }

  vector<set<shared_ptr<const dfa::State>>> stateSets;
  stateSets.push_back(rejectingStates);
  stateSets.push_back(acceptingStates);
  // for(auto state: acceptingStates) {
  //   stateSets.emplace_back(set<shared_ptr<const dfa::State>>{state});
  // }

  return stateSets;
}

}