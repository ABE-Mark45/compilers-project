#include <iostream>
#include <ranges>
#include <vector>
#include <map>
#include <utility>
#include <set>
#include "Parse_Table_Generator.h"
using namespace std;

set<string>getTerminals(const map<string,vector<vector<pair<string ,bool>>>>&m){
    set<string>terminals;
    //loop in map c++ and find if any production goes to terminal or not
    for (const auto& i : m){
        vector<vector<pair<string ,bool>>> vec = i.second;
        for(const auto& j : vec){
            for(auto & k : j){
                if(k.second){
                    terminals.insert(k.first);
                }
            }
        }
    }
    return terminals;
}
set<string>getNonTerminals(const map<string,vector<vector<pair<string ,bool>>>>&m){
    set<string>nonTerminals;
    //loop in map c++ and find if any production goes to terminal or not
    for (const auto& i : m){
        nonTerminals.insert(i.first);
    }
    return nonTerminals;
}


void  Parse_Table_Generator::getFirst(const map<string,vector<vector<pair<string ,bool>>>>&m,
                                      map<basic_string<char>, vector<pair<basic_string<char>, vector<pair<basic_string<char>, bool>>>>>&first){
    //map<string,vector<pair<string,vector<pair<string,bool>>>>>
    //if first is not changed from last loop so there is recursion
    map<string,vector<pair<string,vector<pair<string,bool>>>>>prev_first;
    set<string>nonTerminals = getNonTerminals(m);


    std::vector<string> non_terminals_vector(nonTerminals.begin(), nonTerminals.end());

    while (true) {
        prev_first = first;
        for (auto &it: non_terminals_vector) {
            vector<pair<string,vector<pair<basic_string<char>,bool>>>> state_first;
            vector<vector<pair<string, bool>>> vec = m.find(it)->second;
            for (const auto &j: vec) {
                if(j.empty()){
                    state_first.push_back({"\\L",{}});
                }else {
                    for (auto &k: j) {
                        if (k.second) {
                            //it is terminal so the first is itself
                            state_first.emplace_back(k.first, j);
                            break;//Don't look at the rest before or
                        } else {
                            vector<pair<basic_string<char>, vector<pair<basic_string<char>, bool>>>> first_RHS;
                            if (first.count(k.first) > 0) {
                                first_RHS = first.find(k.first)->second;
                                bool go_to_eps = false;
                                for (const auto &i: first_RHS) {
                                    if (i.first != "\\L") {
                                        state_first.emplace_back(i.first, j);
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
            //Removing duplicates
            vector<pair<string,vector<pair<basic_string<char>,bool>>>> state_first_unique;
            for(int i=0;i<state_first.size();i++){
                bool isUnique = true;
                for(int j = i+1;j<state_first.size();j++){
                    if(state_first[i].first==state_first[j].first&&state_first[i].second==state_first[j].second){
                        isUnique = false;
                    }
                }
                if(isUnique){
                    state_first_unique.push_back(state_first[i]);
                }
            }

            if(!state_first.empty()) {
                if(first.count(it)>0){
                    first.find(it)->second = state_first_unique;
                } else {
                    first.insert({it, state_first_unique});
                }
            }
        }
        if(first == prev_first){
            break;
        }
    }
}
bool containsEps(const vector<pair<basic_string<char>, vector<pair<basic_string<char>, bool>>>>& first){
    for (auto &pair_first: first) {
        if (pair_first.first == "\\L") {
            return true;
        }
    }
    return false;
}
void Parse_Table_Generator::getFollow(const map<string,vector<vector<pair<string ,bool>>>>&m,
                                      map<string,vector<string>>& follow){
//We don't need to know which equation gave us this follow so only this map is good

    map<basic_string<char>, vector<pair<basic_string<char>, vector<pair<basic_string<char>, bool>>>>>first;
    getFirst(m,first);
    set<string>nonTerminals = getNonTerminals(m);
    std::vector<string> non_terminals_vector(nonTerminals.begin(), nonTerminals.end());
    map<string,vector<string>> prev_follow;


    while(true){
        prev_follow = follow;
        for (auto &it: non_terminals_vector) {
            vector<string> state_follow = {};
            if (it == non_terminals_vector[0]) {//if start symbol
                state_follow.emplace_back("$");
            }
            for (auto &it2: non_terminals_vector) {
                //loop on RHS and check it found this nonTerminal
                vector<vector<pair<string, bool>>> vec = m.find(
                        it2)->second;//this is the possible ways this state can go to
                for(auto &prod:vec){
                    for(int i=0;i<prod.size();i++){
                        if(!prod[i].second&&prod[i].first==it){
                            //let's see the symbol which follow it
                            int start = i;
                            if(i+1<prod.size()){
                                i++;
                                if(prod[i].second){
                                    state_follow.push_back(prod[i].first);
                                }
                                else if(!prod[i].second&&!containsEps(first.find(prod[i].first)->second)){
                                    vector<pair<basic_string<char>,
                                            vector<pair<basic_string<char>, bool>>>>first_of_st=first.find(prod[i].first)->second;
                                    for(auto & j : first_of_st){
                                        state_follow.push_back(j.first);
                                    }
                                }
                                else {
                                    while (i < prod.size() &&!prod[i].second && containsEps(first.find(
                                            prod[i].first)->second)) {//while first don't contain epsilon continue to put first in the follow
                                        vector<pair<basic_string<char>,
                                                vector<pair<basic_string<char>, bool>>>> first_of_st = first.find(
                                                prod[i].first)->second;
                                        for (auto &j: first_of_st) {
                                            if (j.first != "\\L") {
                                                state_follow.push_back(j.first);
                                            }
                                        }
                                        i++;
                                    }
                                    if(i==prod.size()){
                                        //put follow of second production
                                        if(follow.count(it2)>0){
                                            for(auto&follow_state:follow.find(it2)->second){
                                                state_follow.push_back(follow_state);
                                            }
                                        }
                                    }else if(prod[i-1].second){
                                        state_follow.push_back(prod[i-1].first);
                                    }else{
                                        //doesn't contain eps
                                        vector<pair<basic_string<char>,
                                                vector<pair<basic_string<char>, bool>>>>first_of_st=first.find(prod[i-1].first)->second;
                                        for(auto & j : first_of_st){
                                            state_follow.push_back(j.first);
                                        }
                                    }
                                }
                                //break;
                                i = start ;
                            }else{
                                //follow is same follow of LHS
                                if(follow.count(it2)>0){
                                    for(auto&follow_state:follow.find(it2)->second){
                                        state_follow.push_back(follow_state);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if(!state_follow.empty()){
                vector<string>follow_unique={};
                for(int i=0;i<state_follow.size();i++){
                    bool unique = true;
                    for(int j=i+1;j<state_follow.size();j++){
                        if(state_follow[i]==state_follow[j]){
                            unique= false;
                        }
                    }
                    if(unique){
                        follow_unique.push_back(state_follow[i]);
                    }
                }
                if(follow.count(it)>0){
                    follow.find(it)->second=follow_unique;
                }else{
                    follow.insert({it,follow_unique});
                }
            }
        }
        if(prev_follow==follow){
            break;
        }
    }
}
map<pair<string/*NT*/,string/*token*/>,vector<pair<string/*NT or Terminal*/,bool>>> Parse_Table_Generator::getTable(const map<string,vector<vector<pair<string ,bool>>>>& m) {
    set<string>terminals = getTerminals(m);
    map<pair<string/*NT*/,basic_string<char>/*token*/>,vector<pair<basic_string<char>/*NT or Terminal*/,bool>>> table;
    map<string,vector<string>>follow;
    map<basic_string<char>, vector<pair<basic_string<char>, vector<pair<basic_string<char>, bool>>>>>first;
    set<string>nonTerminals = getNonTerminals(m);
    std::vector<string> non_terminals_vector(nonTerminals.begin(), nonTerminals.end());
    getFirst(m,first);
    getFollow(m,follow);
    //loop on all first
    for(auto&non_terminal:non_terminals_vector){
        vector<pair<basic_string<char>, vector<pair<basic_string<char>, bool>>>> first_of_non_term = first[non_terminal];
        for(auto&prod:first_of_non_term){
            if(prod.first!="\\L"){
                if(table.count({non_terminal,prod.first})>0 ){
                    cout<<"ERROR Grammar is ambiguous"<<endl;
                    continue;
                }
                //table[{non_terminal,prod.first}] = prod.second;
                table.insert({{non_terminal,prod.first},prod.second});
            }else{
                //put in the follow places this epsilon production
                vector<string>follow_of_non_term = follow[non_terminal];
                for(auto&follow_NT:follow_of_non_term){
                    if(table.count({non_terminal,follow_NT})>0 ){
                        cout<<"ERROR Grammar is ambiguous"<<endl;
                        continue;
                    }
                    table[{non_terminal,follow_NT}] = {};
                }
            }
        }
    }
    //loop on all nonTerminals and follow of them and put sync if cell is empty
    for(auto&non_terminal:non_terminals_vector){
        vector<string>follow_of_non_term = follow[non_terminal];
        for(auto&follow_NT:follow_of_non_term){
            if(table.count({non_terminal,follow_NT})==0){
                table[{non_terminal,follow_NT}] = {{"", true}};//sync
            }
        }
    }
    return table;
}
