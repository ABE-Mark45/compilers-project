#include <gtest/gtest.h>
#include <iostream>
#include "LanguageTokens/Token.h"
#include "utils/AcceptValue.h"
#include "Simulator/ParserSimulator.h"

using namespace std;

#define NT(x) ProductionToken(x, false)
#define T(x) ProductionToken(x, true)


bool compareStackHistory(vector<vector<ProductionToken>> history, vector<vector<ProductionToken>> expected) {
    if(history.size() != expected.size()) {
        return false;
    }
    for(int i = 0; i < history.size(); i++) {
        if(history[i].size() != expected[i].size()) {
            return false;
        }
        for(int j = 0; j < history[i].size(); j++) {
            if(history[i][j] != expected[i][j]) {
                return false;
            }
        }
    }
    return true;
}

TEST(ParserSimulatorTest, testBasicBehaviorAndTerminalMismatch) {
    map<pair<string,string>,ProductionContent> table;
    table[{"S", "a"}] = {NT("A"), T("b"), NT("S")};
    table[{"S", "c"}] = {NT("A"), T("b"), NT("S")};
    table[{"S", "e"}] = {T("e")};
    table[{"S", "$"}] = {};

    table[{"A", "a"}] = {T("a")};
    table[{"A", "b"}] = {SYNC_TOKEN};
    table[{"A", "c"}] = {T("c"), NT("A"), T("d")};
    table[{"A", "d"}] = {SYNC_TOKEN};

    ParserSimulator ps(table, NT("S"));
    string tokens = "aab$";
    for(char c : tokens) {
        ps.consumeToken(string(1, c));
    } 
    cout << endl;
    
    vector<vector<ProductionToken>> expected = {
        {T("$"), NT("S")},
        {T("$"), NT("S"), T("b"), NT("A")},
        {T("$"), NT("S"), T("b"), T("a")},
        {T("$"), NT("S"), T("b")},
        {T("$"), NT("S")},
        {T("$"), NT("S"), T("b"), NT("A")},
        {T("$"), NT("S"), T("b"), T("a")},
        {T("$"), NT("S"), T("b")},
        {T("$"), NT("S")},
        {T("$")}
    };

    // print simulation history
    // cout << "simulation history:\n";
    // for(auto v : ps.getHistory()) {
    //     for(auto pt : v) {
    //         cout << pt.first << " ";
    //     }
    //     cout << endl;
    // }

    ASSERT_TRUE(compareStackHistory(ps.getHistory(), expected));
    
}


TEST(ParserSimulatorTest, testNonTerminalMismatch) {
    map<pair<string,string>,ProductionContent> table;
    table[{"S", "a"}] = {NT("A"), T("b"), NT("S")};
    table[{"S", "c"}] = {NT("A"), T("b"), NT("S")};
    table[{"S", "e"}] = {T("e")};
    table[{"S", "$"}] = {};

    table[{"A", "a"}] = {T("a")};
    table[{"A", "b"}] = {SYNC_TOKEN};
    table[{"A", "c"}] = {T("c"), NT("A"), T("d")};
    table[{"A", "d"}] = {SYNC_TOKEN};

    ParserSimulator ps(table, NT("S"));
    string tokens = "ceadb$";
    for(char c : tokens) {
        ps.consumeToken(string(1, c));
    } 
    cout << endl;
    
    vector<vector<ProductionToken>> expected = {
        {T("$"), NT("S")},
        {T("$"), NT("S"), T("b"), NT("A")},
        {T("$"), NT("S"), T("b"), T("d"), NT("A"), T("c")},
        {T("$"), NT("S"), T("b"), T("d"), NT("A")},
        {T("$"), NT("S"), T("b"), T("d"), NT("A")},
        {T("$"), NT("S"), T("b"), T("d"), T("a")},
        {T("$"), NT("S"), T("b"), T("d")},
        {T("$"), NT("S"), T("b")},
        {T("$"), NT("S")},
        {T("$")}
    };

    // print simulation history
    // cout << "simulation history:\n";
    // for(auto v : ps.getHistory()) {
    //     for(auto pt : v) {
    //         cout << pt.first << " ";
    //     }
    //     cout << endl;
    // }

    ASSERT_TRUE(compareStackHistory(ps.getHistory(), expected));
    
}

TEST(ParserSimulatorTest, SyncProduction) {
    map<pair<string,string>,ProductionContent> table;
    table[{"S", "a"}] = {NT("A"), T("b"), NT("S")};
    table[{"S", "c"}] = {NT("A"), T("b"), NT("S")};
    table[{"S", "e"}] = {T("e")};
    table[{"S", "$"}] = {};

    table[{"A", "a"}] = {T("a")};
    table[{"A", "b"}] = {SYNC_TOKEN};
    table[{"A", "c"}] = {T("c"), NT("A"), T("d")};
    table[{"A", "d"}] = {SYNC_TOKEN};

    ParserSimulator ps(table, NT("S"));
    string tokens = "cdb$";
    for(char c : tokens) {
        ps.consumeToken(string(1, c));
    } 
    cout << endl;
    
    vector<vector<ProductionToken>> expected = {
        {T("$"), NT("S")},
        {T("$"), NT("S"), T("b"), NT("A")},
        {T("$"), NT("S"), T("b"), T("d"), NT("A"), T("c")},
        {T("$"), NT("S"), T("b"), T("d"), NT("A")},
        {T("$"), NT("S"), T("b"), T("d")},
        {T("$"), NT("S"), T("b")},
        {T("$"), NT("S")},
        {T("$")}
    };

    // print simulation history
    // cout << "simulation history:\n";
    // for(auto v : ps.getHistory()) {
    //     for(auto pt : v) {
    //         cout << pt.first << " ";
    //     }
    //     cout << endl;
    // }

    ASSERT_TRUE(compareStackHistory(ps.getHistory(), expected));
    
}