#include <gtest/gtest.h>
#include <iostream>
#include "ParseTableGenerator/ParseTableGenerator.h"

using namespace std;

TEST(ParseTableGeneratorTest, firstTest) {
    map<string, vector<vector<pair<string, bool>>>> m;
    map<basic_string<char>, vector<pair<basic_string<char>, vector<pair<basic_string<char>, bool>>>>> first;
    string E = "E";
    string T = "T";
    string T_dash = "T'";
    string E_dash = "E'";
    string F = "F";

    m.insert({E,
              {{{T, false}, {E_dash, false}}}});
    m.insert({E_dash, {{{"+", true}, {T, false}, {E_dash, false}}, {}}});
    m.insert({T, {{{F, false}, {T_dash, false}}}});
    m.insert({T_dash, {{{"*", true}, {F, false}, {T_dash, false}}, {}}});
    m.insert({F, {{{"(", true}, {E, false}, {")", true}}, {{"id", true}}}});
    ParseTableGenerator::getFirst(m, first);
    map<basic_string<char>, vector<pair<basic_string<char>, vector<pair<basic_string<char>, bool>>>>> expectedFirst =
            {
                    {"E",      {{"(", {{T,   false}, {E_dash, false}}},                  {"id",  {{T,    false}, {E_dash, false}}}}},
                    {"E_dash", {{"+", {{"+", true},  {T,      false}, {E_dash, false}}}, {"\\L", {}}}},
                    {"F",      {{"(", {{"(", true},  {E,      false}, {")",    true}}},  {"id",  {{"id", true}}}}},
                    {"T",      {{"(", {{F,   false}, {T_dash, false}}},                  {"id",  {{F,    false}, {T_dash, false}}}}},
                    {"T_dash", {{"*", {{"*", true},  {F,      false}, {T_dash, false}}}, {"\\L", {}}}}
            };
    map<basic_string<char>, vector<pair<basic_string<char>, vector<pair<basic_string<char>, bool>>>>>::iterator itr;
    for (itr = first.begin(); itr != first.end(); itr++) {
        map<basic_string<char>, vector<pair<basic_string<char>, vector<pair<basic_string<char>, bool>>>>>::iterator itr2;

        for (itr2 = expectedFirst.begin(); itr2 != expectedFirst.end(); itr2++) {
            if (itr->first == itr2->first) {
                //ASSERT_EQ(itr->first,itr2->first);
                ASSERT_EQ(itr->second, itr2->second);
                break;
            }
        }
    }
}

TEST(ParseTableGeneratorTest, followTest) {
    map<string, vector<vector<pair<string, bool>>>> m;
    map<string, vector<string>> follow;
    string E = "E";
    string T = "T";
    string T_dash = "T'";
    string E_dash = "E'";
    string F = "F";

    m.insert({E,
              {{{T, false}, {E_dash, false}}}});
    m.insert({E_dash, {{{"+", true}, {T, false}, {E_dash, false}}, {}}});
    m.insert({T, {{{F, false}, {T_dash, false}}}});
    m.insert({T_dash, {{{"*", true}, {F, false}, {T_dash, false}}, {}}});
    m.insert({F, {{{"(", true}, {E, false}, {")", true}}, {{"id", true}}}});
    ParseTableGenerator::getFollow(m, follow, "E");
    map<string, vector<string>> expected_follow = {
            {"E",      {"$", ")"}},
            {"E_dash", {"$", ")"}},
            {"F",      {"*", "+", "$", ")"}},
            {"T",      {"+", "$", ")"}},
            {"T_dash", {}}
    };
    map<string, vector<string>>::iterator itr;
    for (itr = follow.begin(); itr != follow.end(); itr++) {
        map<string, vector<string>>::iterator itr2;
        for (itr2 = expected_follow.begin(); itr2 != expected_follow.end(); itr2++) {
            if (itr->first == itr2->first) {
                //ASSERT_EQ(itr->first,itr2->first);
                ASSERT_EQ(itr->second, itr2->second);
                break;
            }
        }
    }
}

TEST(ParseTableGeneratorTest, tableTest) {
    map<string, vector<vector<pair<string, bool>>>> m;
    string E = "E";
    string T = "T";
    string T_dash = "T'";
    string E_dash = "E'";
    string F = "F";

    m.insert({E,
              {{{T, false}, {E_dash, false}}}});
    m.insert({E_dash, {{{"+", true}, {T, false}, {E_dash, false}}, {}}});
    m.insert({T, {{{F, false}, {T_dash, false}}}});
    m.insert({T_dash, {{{"*", true}, {F, false}, {T_dash, false}}, {}}});
    m.insert({F, {{{"(", true}, {E, false}, {")", true}}, {{"id", true}}}});
    map<pair<string/*NT*/, string/*token*/>, vector<pair<string/*NT or Terminal*/, bool>>> table;
    table = ParseTableGenerator::getTable(m, "E");
    map<pair<string/*NT*/, string/*token*/>, vector<pair<string/*NT or Terminal*/, bool>>> expectedTable =
            {
                    {{"E",  "$"},  {{"#",  true}}},
                    {{"E",  "("},  {{T,    false}, {E_dash, false}}},
                    {{"E",  ")"},  {{"#",  true}}},
                    {{"E",  "id"}, {{T,    false}, {E_dash, false}}},

                    {{"E'", "$"},  {}},
                    {{"E'", ")"},  {}},
                    {{"E'", "+"},  {{"+",  true},  {T,      false}, {E_dash, false}}},

                    {{"T",  "$"},  {{"#",  true}}},
                    {{"T",  "("},  {{F,    false}, {T_dash, false}}},
                    {{"T",  ")"},  {{"#",  true}}},
                    {{"T",  "+"},  {{"#",  true}}},
                    {{"T",  "id"}, {{F,    false}, {T_dash, false}}},

                    {{"T'", "$"},  {}},
                    {{"T'", ")"},  {}},
                    {{"T'", "*"},  {{"*",  true},  {F,      false}, {T_dash, false}}},
                    {{"T'", "+"},  {}},

                    {{"F",  "$"},  {{"#",  true}}},
                    {{"F",  "("},  {{"(",  true},  {"E",    false}, {")",    true}}},
                    {{"F",  ")"},  {{"#",  true}}},
                    {{"F",  "*"},  {{"#",  true}}},
                    {{"F",  "+"},  {{"#",  true}}},
                    {{"F",  "id"}, {{"id", true}}}
            };
    map<pair<string/*NT*/, string/*token*/>, vector<pair<string/*NT or Terminal*/, bool>>>::iterator itr;
    ASSERT_EQ(table.size(), expectedTable.size());
    for (itr = table.begin(); itr != table.end(); itr++) {
        map<pair<string/*NT*/, string/*token*/>, vector<pair<string/*NT or Terminal*/, bool>>>::iterator itr2;
        bool found = false;
        for (itr2 = expectedTable.begin(); itr2 != expectedTable.end(); itr2++) {
            if (itr->first == itr2->first) {
                //ASSERT_EQ(itr->first,itr2->first);
                ASSERT_EQ(itr->second, itr2->second);
                found = true;
                break;
            }
        }
        if (!found) {
            cout << itr->first.first << " " << itr->first.second << " ";
            cout << "Not found" << endl;
        }
    }

}
