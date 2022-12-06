#pragma once
#include <DFA/State.h>
#include <memory>
#include <ostream>
#include <string>
#include <unordered_set>

static const std::string kSep = ",";

static std::string escapeString(std::string_view str) {
  std::string ret = "\"";
  for (char c : str) {
    if (c == '\"') {
      ret += "\"\"";
    } else {
      ret.push_back(c);
    }
  }

  ret.push_back('\"');
  return ret;
}

static void printDFA(
    std::shared_ptr<const dfa::State> state, std::ostream& o,
    std::unordered_set<std::shared_ptr<const dfa::State>>& visited) {
  visited.insert(state);
  std::string str{std::to_string(state->getId())};

  if (state->getAcceptValue()) {
    str += kSep + escapeString(state->getAcceptValue().value().value);
  } else {
    str += kSep + "_";
  }

  for (unsigned char c = 33; c <= 126; c++) {
    str += kSep;
    if (auto nextState = state->moveThrough(c)) {
      str += std::to_string(nextState->getId());
    } else {
      str += "_";
    }
  }

  o << str << '\n';

  for (unsigned char c = 33; c <= 126; c++) {
    if (auto nextState = state->moveThrough(c)) {
      if (visited.count(nextState) == 0) {
        printDFA(nextState, o, visited);
      }
    }
  }
}

void printDFA(std::shared_ptr<const dfa::State> state, std::ostream& o) {
  std::unordered_set<std::shared_ptr<const dfa::State>> visited;

  o << '_' << kSep << '_';
  for (char i = 33; i <= 126; i++) {
    o << kSep << escapeString(std::string(1, i));
  }
  o << '\n';
  printDFA(state, o, visited);
}