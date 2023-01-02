#include "ParseTableGenerator.h"
#include <iostream>
#include <map>
#include <ranges>
#include <set>
#include <unordered_set>
#include <utility>
#include <vector>
using namespace std;
bool is_ambigious = false;
set<string> getTerminals(const ProductionsTable& productionsTable) {
  set<string> terminals;
  //loop in map c++ and find if any production goes to terminal or not
  for (const auto& i : productionsTable) {
    vector<vector<pair<string, bool>>> vec = i.second;
    for (const auto& j : vec) {
      for (auto& k : j) {
        if (k.second) {
          terminals.insert(k.first);
        }
      }
    }
  }
  return terminals;
}
unordered_set<string> getNonTerminals(
    const ProductionsTable& productionsTable) {
  unordered_set<string> nonTerminals;
  //loop in map c++ and find if any production goes to terminal or not
  for (const auto& i : productionsTable) {
    nonTerminals.insert(i.first);
  }
  return nonTerminals;
}

FirstMap ParseTableGenerator::getFirstMap(
    const ProductionsTable& productionsTable) {
  //unordered_map<string,vector<pair<string,vector<pair<string,bool>>>>>
  //if first is not changed from last loop so there is recursion
  FirstMap first, prev_first;
  unordered_set<string> nonTerminals = getNonTerminals(productionsTable);

  while (true) {
    prev_first = first;
    for (auto& it : nonTerminals) {
      FirstSet state_first;
      auto vec = productionsTable.find(it)->second;
      for (const auto& j : vec) {
        if (j.empty()) {
          state_first.insert({"\\L", {}});
        } else {
          for (auto& k : j) {
            if (k.second) {
              //it is terminal so the first is itself
              state_first.insert({k.first, j});
              break;  //Don't look at the rest before or
            } else {
              FirstSet first_RHS;
              if (first.count(k.first) > 0) {
                first_RHS = first.find(k.first)->second;
                bool go_to_eps = false;
                for (const auto& i : first_RHS) {
                  if (i.first != "\\L") {
                    state_first.insert({i.first, j});
                  } else {
                    go_to_eps = true;
                  }
                }
                if (!go_to_eps) {
                  break;
                }
              }
            }
          }
        }
      }

      if (!state_first.empty()) {
        first[it] = state_first;
      }
    }
    if (first == prev_first) {
      break;
    }
  }
  return first;
}

bool containsEps(const FirstSet& first) {
  for (auto& pair_first : first) {
    if (pair_first.first == "\\L") {
      return true;
    }
  }
  return false;
}

FollowMap ParseTableGenerator::getFollowMap(
    const ProductionsTable& productionsTable, const FirstMap& firstMap,
    const string& start_symbol) {
  FollowMap follow;
  //We don't need to know which equation gave us this follow so only this map is good
  unordered_set<string> nonTerminals = getNonTerminals(productionsTable);
  FollowMap prev_follow;

  while (true) {
    prev_follow = follow;
    for (auto& it : nonTerminals) {
      unordered_set<string> state_follow;
      if (it == start_symbol) {  //if start symbol
        state_follow.emplace("$");
      }
      for (auto& it2 : nonTerminals) {
        //loop on RHS and check it found this nonTerminal
        auto vec = productionsTable.at(
            it2);  //this is the possible ways this state can go to
        for (auto& prod : vec) {
          for (int i = 0; i < prod.size(); i++) {
            if (!prod[i].second && prod[i].first == it) {
              //let's see the symbol which follow it
              int start = i;
              if (i + 1 < prod.size()) {
                i++;
                if (prod[i].second) {
                  state_follow.emplace(prod[i].first);
                } else if (!prod[i].second &&
                           !containsEps(firstMap.find(prod[i].first)->second)) {
                  auto first_of_st = firstMap.find(prod[i].first)->second;
                  for (auto& j : first_of_st) {
                    state_follow.emplace(j.first);
                  }
                } else {
                  while (
                      i < prod.size() && !prod[i].second &&
                      containsEps(
                          firstMap.find(prod[i].first)
                              ->second)) {  //while first don't contain epsilon continue to put first in the follow
                    auto first_of_st = firstMap.find(prod[i].first)->second;
                    for (auto& j : first_of_st) {
                      if (j.first != "\\L") {
                        state_follow.emplace(j.first);
                      }
                    }
                    i++;
                  }
                  if (i == prod.size()) {
                    //put follow of second production
                    if (follow.count(it2) > 0) {
                      for (auto& follow_state : follow.find(it2)->second) {
                        state_follow.emplace(follow_state);
                      }
                    }
                  } else if (prod[i - 1].second) {
                    state_follow.emplace(prod[i - 1].first);
                  } else {
                    //doesn't contain eps
                    auto first_of_st = firstMap.find(prod[i - 1].first)->second;
                    for (auto& j : first_of_st) {
                      state_follow.emplace(j.first);
                    }
                  }
                }
                //break;
                i = start;
              } else {
                //follow is same follow of LHS
                if (follow.count(it2) > 0) {
                  for (auto& follow_state : follow.find(it2)->second) {
                    state_follow.emplace(follow_state);
                  }
                }
              }
            }
          }
        }
      }
      if (!state_follow.empty()) {
        follow[it] = std::move(state_follow);
      }
    }
    if (prev_follow == follow) {
      break;
    }
  }
  return follow;
}

ParseTable ParseTableGenerator::getTable(
    const ProductionsTable& productionsTable, const string& start_symbol) {
  set<string> terminals = getTerminals(productionsTable);
  ParseTable table;
  auto nonTerminals = getNonTerminals(productionsTable);
  const auto first = getFirstMap(productionsTable);
  const auto follow = getFollowMap(productionsTable, first, start_symbol);
  //loop on all first
  for (auto& non_terminal : nonTerminals) {
    auto first_of_non_term = first.at(non_terminal);
    for (auto& prod : first_of_non_term) {
      if (prod.first != "\\L") {
        if (table.count({non_terminal, prod.first}) > 0) {
          is_ambigious = true;
          cout << "ERROR Grammar is ambiguous" << endl;
          continue;
        }
        //table[{non_terminal,prod.first}] = prod.second;
        table.insert({{non_terminal, prod.first}, prod.second});
      } else {
        //put in the follow places this epsilon production
        auto follow_of_non_term = follow.at(non_terminal);
        for (auto& follow_NT : follow_of_non_term) {
          if (table.count({non_terminal, follow_NT}) > 0) {
            cout << "ERROR Grammar is ambiguous" << endl;
            is_ambigious = true;
            continue;
          }
          table[{non_terminal, follow_NT}] = {};
        }
      }
    }
  }
  //loop on all nonTerminals and follow of them and put sync if cell is empty
  for (auto& non_terminal : nonTerminals) {
    auto follow_of_non_term = follow.at(non_terminal);
    for (auto& follow_NT : follow_of_non_term) {
      if (table.count({non_terminal, follow_NT}) == 0) {
        table[{non_terminal, follow_NT}] = {{"#", true}};  //sync
      }
    }
  }
  //So put sync in the syncBeginStatements if empty cell
  /*
    vector<string>syncBeginStatements = {";","if","do","while","for","{","}"};
    for(auto&non_terminal:nonTerminals){
        vector<string>follow_of_non_term = follow[non_terminal];
        for(auto&terminal:terminals_vector){
            if(table.count({non_terminal,terminal})==0&&
            std::count(syncBeginStatements.begin(), syncBeginStatements.end(), terminal)){
                table[{non_terminal,terminal}] = {{"#", true}};//sync
            }
        }
    }
     //if epsilon is default transition
     for(auto&non_terminal:nonTerminals){
        vector<pair<string, vector<pair<string, bool>>>> first_of_non_term = first[non_terminal];
        if(containsEps(first_of_non_term)){
            for(auto&terminal:terminals_vector){
                if(table.count({non_terminal,terminal})==0){
                    table[{non_terminal,terminal}] = {};
                }
            }
        }
    }
     */

  return table;
}

bool ParseTableGenerator::get_is_ambiguous() {
  return is_ambigious;
}
