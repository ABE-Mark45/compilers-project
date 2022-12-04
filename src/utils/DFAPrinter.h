#pragma once
#include <DFA/State.h>
#include <memory>
#include <ostream>
#include <string>
#include <unordered_set>

static void printDFA(
    std::shared_ptr<const dfa::State> state, std::ostream& o,
    std::unordered_set<std::shared_ptr<const dfa::State>>& visited) {
  visited.insert(state);
  std::string str{std::to_string(state->getId())};

  if (state->getAcceptValue()) {
    str += "," + state->getAcceptValue().value().value + ",";
  } else {
    str += ",_,";
  }

  for (unsigned char c = 1; c <= 127; c++) {
    if (auto nextState = state->moveThrough(c)) {
      str += std::to_string(nextState->getId());
    } else {
      str += "-1";
    }
    str.push_back(',');
  }

  str.pop_back();
  o << str << '\n';

  for (unsigned char c = 1; c <= 127; c++) {
    if (auto nextState = state->moveThrough(c)) {
      if (visited.count(nextState) == 0) {
        printDFA(nextState, o, visited);
      }
    }
  }
}

void printDFA(std::shared_ptr<const dfa::State> state, std::ostream& o) {
  std::unordered_set<std::shared_ptr<const dfa::State>> visited;

  o << '_';
  for (int i = 1; i <= 127; i++) {
    o << ',' << std::to_string(i);
  }
  o << '\n';
  printDFA(state, o, visited);
}