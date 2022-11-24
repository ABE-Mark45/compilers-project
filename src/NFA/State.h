#pragma once

#include <LanguageTokens/Token.h>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

class State {

  // Computes epsilon closure of the current state given the mapping between each state ID and its internal data
  std::set<int> epsilonClosure(
      std::unordered_map<int, std::shared_ptr<State>>& idToState) const;

  // Computes epsilon closure of the current state after moving through
  // the specified transition given the mapping between each state ID and its internal data
  std::set<int> moveThrough(
      char transition,
      std::unordered_map<int, std::shared_ptr<State>>& idToState) const;

 private:
  // A utility function for the epsilon closure
  void epsilonClosure(
      int id, std::set<int>& closure,
      std::unordered_map<int, std::shared_ptr<State>>& idToState) const;

  // State ID
  int id_;
  // mapping between transitions and state IDs
  std::unordered_map<char, std::vector<int>> transitions_;
};