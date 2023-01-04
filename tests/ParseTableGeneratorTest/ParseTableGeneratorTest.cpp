#include <gtest/gtest.h>
#include <utils/CFGTypes.h>
#include "ParseTableGenerator/ParseTableGenerator.h"

using namespace std;

TEST(ParseTableGeneratorTest, firstTest) {
    ProductionsTable m;
    const string E = "E";
    const string T = "T";
    const string T_dash = "T'";
    const string E_dash = "E'";
    const string F = "F";

    m.insert({E, {{{T, false}, {E_dash, false}}}});
    m.insert({E_dash, {{{"+", true}, {T, false}, {E_dash, false}}, {}}});
    m.insert({T, {{{F, false}, {T_dash, false}}}});
    m.insert({T_dash, {{{"*", true}, {F, false}, {T_dash, false}}, {}}});
    m.insert({F, {{{"(", true}, {E, false}, {")", true}}, {{"id", true}}}});

    const auto first = ParseTableGenerator::getFirstMap(m);
    const FirstMap expectedFirst = {
            {"E",
                    {{"(", {{T,   false}, {E_dash, false}}},
                                                                              {"id",  {{T,    false}, {E_dash, false}}}}},
            {"E\'", {{"+", {{"+", true},  {T,      false}, {E_dash, false}}}, {"\\L", {}}}},
            {"F",
                    {{"(", {{"(", true},  {E,      false}, {")",    true}}},  {"id",  {{"id", true}}}}},
            {"T",
                    {{"(", {{F,   false}, {T_dash, false}}},
                                                                              {"id",  {{F,    false}, {T_dash, false}}}}},
            {"T\'",
                    {{"*", {{"*", true},  {F,      false}, {T_dash, false}}}, {"\\L", {}}}}};
    ASSERT_EQ(first, expectedFirst);
}

TEST(ParseTableGeneratorTest, followTest) {
    ProductionsTable m;
    const string E = "E";
    const string T = "T";
    const string T_dash = "T'";
    const string E_dash = "E'";
    const string F = "F";

    m.insert({E, {{{T, false}, {E_dash, false}}}});
    m.insert({E_dash, {{{"+", true}, {T, false}, {E_dash, false}}, {}}});
    m.insert({T, {{{F, false}, {T_dash, false}}}});
    m.insert({T_dash, {{{"*", true}, {F, false}, {T_dash, false}}, {}}});
    m.insert({F, {{{"(", true}, {E, false}, {")", true}}, {{"id", true}}}});

    const auto first = ParseTableGenerator::getFirstMap(m);
    const auto follow = ParseTableGenerator::getFollowMap(m, first, "E");
    const FollowMap expected_follow = {{"E",   {"$", ")"}},
                                       {"E\'", {"$", ")"}},
                                       {"F",   {"*", "+", "$", ")"}},
                                       {"T",   {"+", "$", ")"}},
                                       {"T\'", {"+", ")", "$"}}};
    ASSERT_EQ(follow, expected_follow);
}

TEST(firstTest2, first2) {
    //A → abc / def / ghi
    string A = "A";
    ProductionsTable m;
    m.insert({A, {{{"a", true}, {"b", true}, {"c", true}}, {{"d", true}, {"e", true}, {"f", true}}}});
    const auto first = ParseTableGenerator::getFirstMap(m);
    const FirstMap expectedFirst = {
            {A, {{"a", {{"a", true}, {"b", true}, {"c", true}}}, {"d", {{"d", true}, {"e", true}, {"f", true}}}}}};
    ASSERT_EQ(first, expectedFirst);
}

TEST(followTest2, follow2) {
    //A → abc / def / ghi
    string A = "A";
    ProductionsTable m;
    m.insert({A, {{{"a", true}, {"b", true}, {"c", true}}, {{"d", true}, {"e", true}, {"f", true}}}});
    const auto first = ParseTableGenerator::getFirstMap(m);
    const auto follow = ParseTableGenerator::getFollowMap(m, first, "A");
    const FollowMap expected_follow = {{A, {"$"}}};
    ASSERT_EQ(follow, expected_follow);
}

TEST(firstTest3, first3) {
    string S = "S";
    string B = "B";
    string C = "C";
    string D = "D";
    string E = "E";
    string F = "F";
    ProductionsTable m;
    m[S] = {{{"a", true}, {B, false}, {D, false}, {"h", true}}};
    m[B] = {{{"c", true}, {C, false}}};
    m[C] = {{{"b", true}, {C, false}},
            {}};
    m[D] = {{{E, false}, {F, false}}};
    m[E] = {{{"g", true}},
            {}};
    m[F] = {{{"f", true}},
            {}};
    const auto first = ParseTableGenerator::getFirstMap(m);
    const FirstMap expectedFirst = {{S, {{"a", {{"a", true},  {B, false}, {D, false}, {"h", true}}}}},
                                    {B, {{"c", {{"c", true},  {C, false}}}}},
                                    {C, {{"b", {{"b", true},  {C, false}}}, {"\\L", {}}}},
                                    {D, {{"g", {{E,   false}, {F, false}}}, {"f",   {{E, false}, {F, false}}}, {"\\L", {}}}},
                                    {E, {{"g", {{"g", true}}},              {"\\L", {}}}},
                                    {F, {{"f", {{"f", true}}},              {"\\L", {}}}}};
    ASSERT_EQ(first, expectedFirst);
}

TEST(followTest3, follow3) {
    string S = "S";
    string B = "B";
    string C = "C";
    string D = "D";
    string E = "E";
    string F = "F";
    ProductionsTable m;
    m[S] = {{{"a", true}, {B, false}, {D, false}, {"h", true}}};
    m[B] = {{{"c", true}, {C, false}}};
    m[C] = {{{"b", true}, {C, false}},
            {}};
    m[D] = {{{E, false}, {F, false}}};
    m[E] = {{{"g", true}},
            {}};
    m[F] = {{{"f", true}},
            {}};
    const auto first = ParseTableGenerator::getFirstMap(m);
    const auto follow = ParseTableGenerator::getFollowMap(m, first, "S");
    const FollowMap expected_follow = {{S, {"$"}},
                                       {B, {"g", "f", "h"}},
                                       {C, {"g", "f", "h"}},
                                       {D, {"h"}},
                                       {E, {"f", "h"}},
                                       {F, {"h"}}};
    ASSERT_EQ(follow, expected_follow);
}


TEST(firstTest4, first4) {
    string S = "S";
    string A = "A";
    string A_dash = "A'";
    string B = "B";
    string C = "C";
    ProductionsTable m;
    m[S] = {{{A, false}}};
    m[A] = {{{"a", true}, {B, false}, {A_dash, false}}};
    m[A_dash] = {{{"d", true}, {A_dash, false}},
                 {}};
    m[B] = {{{"b", true}}};
    m[C] = {{{"g", true}}};
    const auto first = ParseTableGenerator::getFirstMap(m);
    const FirstMap expectedFirst = {{S,      {{"a", {{A,   false}}}}},
                                    {A,      {{"a", {{"a", true}, {B,      false}, {A_dash, false}}}}},
                                    {A_dash, {{"d", {{"d", true}, {A_dash, false}}}, {"\\L", {}}}},
                                    {B,      {{"b", {{"b", true}}}}},
                                    {C,      {{"g", {{"g", true}}}}}};
    ASSERT_EQ(first, expectedFirst);
}

TEST(followTest4, follow4) {
    string S = "S";
    string A = "A";
    string A_dash = "A'";
    string B = "B";
    string C = "C";
    ProductionsTable m;
    m[S] = {{{A, false}}};
    m[A] = {{{"a", true}, {B, false}, {A_dash, false}}};
    m[A_dash] = {{{"d", true}, {A_dash, false}},
                 {}};
    m[B] = {{{"b", true}}};
    m[C] = {{{"g", true}}};
    const auto first = ParseTableGenerator::getFirstMap(m);
    const auto follow = ParseTableGenerator::getFollowMap(m, first, "S");
    const FollowMap expected_follow = {{S,      {"$"}},
                                       {A,      {"$"}},
                                       {A_dash, {"$"}},
                                       {B,      {"d", "$"}}};
    ASSERT_EQ(follow, expected_follow);
}


TEST(firstTest5, first5) {
    string S = "S";
    string L = "L";
    string L_dash = "L'";
    ProductionsTable m;
    m[S] = {{{"(", true}, {L, false}, {")", true}},
            {{"a", true}}};
    m[L] = {{{S, false}, {L_dash, false}}};
    m[L_dash] = {{{",", true}, {S, false}, {L_dash, false}},
                 {}};
    const auto first = ParseTableGenerator::getFirstMap(m);
    const FirstMap expectedFirst = {{S,      {{"(", {{"(", true},  {L,      false}, {")",    true}}},  {"a",   {{"a", true}}}}},
                                    {L,      {{"(", {{S,   false}, {L_dash, false}}},                  {"a",   {{S,   false}, {L_dash, false}}}}},
                                    {L_dash, {{",", {{",", true},  {S,      false}, {L_dash, false}}}, {"\\L", {}}}}};
    ASSERT_EQ(first, expectedFirst);
}

TEST(followTest5, follow5) {
    string S = "S";
    string L = "L";
    string L_dash = "L'";
    ProductionsTable m;
    m[S] = {{{"(", true}, {L, false}, {")", true}},
            {{"a", true}}};
    m[L] = {{{S, false}, {L_dash, false}}};
    m[L_dash] = {{{",", true}, {S, false}, {L_dash, false}},
                 {}};
    const auto first = ParseTableGenerator::getFirstMap(m);
    const auto follow = ParseTableGenerator::getFollowMap(m, first, "S");
    const FollowMap expected_follow = {{S,      {"$", ",", ")"}},
                                       {L,      {")"}},
                                       {L_dash, {")"}}};
    ASSERT_EQ(follow, expected_follow);
}


TEST(firstTest6, first6) {
    string S = "S";
    string A = "A";
    string B = "B";
    ProductionsTable m;
    m[S] = {{{A, false}, {"a", true}, {A, false}, {"b", true}},
            {{B, false}, {"b", true}, {B, false}, {"a", true}}};
    m[A] = {{}};
    m[B] = {{}};
    const auto first = ParseTableGenerator::getFirstMap(m);
    const FirstMap expectedFirst = {{S, {{"a",   {{A, false}, {"a", true}, {A, false}, {"b", true}}}, {"b", {{B, false}, {"b", true}, {B, false}, {"a", true}}}}},
                                    {A, {{"\\L", {}}}},
                                    {B, {{"\\L", {}}}}};
    ASSERT_EQ(first, expectedFirst);
}

TEST(followTest6, follow6) {
    string S = "S";
    string A = "A";
    string B = "B";
    ProductionsTable m;
    m[S] = {{{A, false}, {"a", true}, {A, false}, {"b", true}},
            {{B, false}, {"b", true}, {B, false}, {"a", true}}};
    m[A] = {{}};
    m[B] = {{}};
    const auto first = ParseTableGenerator::getFirstMap(m);

    const auto follow = ParseTableGenerator::getFollowMap(m, first, "S");
    const FollowMap expected_follow = {{S, {"$"}},
                                       {A, {"a", "b"}},
                                       {B, {"a", "b"}}};
    ASSERT_EQ(follow, expected_follow);
}

TEST(firstTest7, first7) {
    string S = "S";
    string A = "A";
    string B = "B";
    string C = "C";
    ProductionsTable m;
    m[S] = {{{A, false},{C, false},{B, false}},{{C, false},{"b", true},{B, false}},{{B, false},{"a", true}}};
    m[A] = {{{"d", true},{"a", true}},{{B, false},{C, false}}};
    m[B] = {{{"g", true}},{}};
    m[C] = {{{"h", true}},{}};
    const auto first = ParseTableGenerator::getFirstMap(m);
    const FirstMap expectedFirst = {{A,{{"d",{{"d", true},{"a", true}}},{"g",{{B, false},{C, false}}},{"h",{{B, false},{C, false}}},{"\\L",{}}}},
                                    {B,{{"g",{{"g", true}}},{"\\L",{}}}},
                                    {C,{{"h",{{"h", true}}},{"\\L",{}}}},
                                    {S, {{"d",{{A, false},{C, false},{B, false}}},
                                    {"g",{{A, false},{C, false},{B, false}}},{"g",{{B, false},{"a", true}}},
                                    {"h",{{A, false},{C, false},{B, false}}},{"h",{{C, false},{"b", true},{B, false}}},
                                    {"\\L",{}},
                                    {"a",{{B, false},{"a", true}}},
                                    {"b",{{C, false},{"b", true},{B, false}}}}}};
    ASSERT_EQ(first, expectedFirst);
}

TEST(followTest7, follow7) {
    string S = "S";
    string A = "A";
    string B = "B";
    string C = "C";
    ProductionsTable m;
    m[S] = {{{A, false},{C, false},{B, false}},{{C, false},{"b", true},{B, false}},{{B, false},{"a", true}}};
    m[A] = {{{"d", true},{"a", true}},{{B, false},{C, false}}};
    m[B] = {{{"g", true}},{}};
    m[C] = {{{"h", true}},{}};
    const auto first = ParseTableGenerator::getFirstMap(m);
    const auto follow = ParseTableGenerator::getFollowMap(m, first, "S");
    const FollowMap expected_follow = {{S, {"$"}},
                                       {A, {"h","g","$"}},
                                       {B, {"$","a","h","g"}},
                                       {C, {"g","$","b","h"}}};
    ASSERT_EQ(follow, expected_follow);
}

TEST(ParseTableGeneratorTest, tableTest) {
    ProductionsTable m;
    const string E = "E";
    const string T = "T";
    const string T_dash = "T'";
    const string E_dash = "E'";
    const string F = "F";

    m.insert({E, {{{T, false}, {E_dash, false}}}});
    m.insert({E_dash, {{{"+", true}, {T, false}, {E_dash, false}}, {}}});
    m.insert({T, {{{F, false}, {T_dash, false}}}});
    m.insert({T_dash, {{{"*", true}, {F, false}, {T_dash, false}}, {}}});
    m.insert({F, {{{"(", true}, {E, false}, {")", true}}, {{"id", true}}}});

    const ParseTable table = ParseTableGenerator::getTable(m, "E");

    const ParseTable expectedTable = {
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
            {{"F",  "id"}, {{"id", true}}}};

    ASSERT_EQ(table, expectedTable);
}
