#include <DFA/DFA.h>

DFA::DFA(std::shared_ptr<NFA::NFA> NFA)
    : startState_(std::make_shared<State>())) {
  // TODO subset construction
  // create a map from set<NFA_state_ids> to map<char, set<NFA_state_ids>>
  // create a stack of set<NFA_state_ids> with DFA.start_state, which is the e-closure of NFA
  // while the stack is not empty:
  //    for each character in the language
  //        add a transition from top of stack to e-closure(NFA transition)
  //        if the new closure is not in the map, add it to the stack

  // for each entry in the map, create a DFA state and copy the transitions?
}

std::unique_ptr<DFA> DFA::minimize(){

}

std::string DFA::consume(int *ptr, std::string input){

}