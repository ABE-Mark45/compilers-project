#include <gtest/gtest.h>
#include <utils/CFGTypes.h>
#include <iostream>
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
       {{"(", {{T, false}, {E_dash, false}}},
        {"id", {{T, false}, {E_dash, false}}}}},
      {"E\'", {{"+", {{"+", true}, {T, false}, {E_dash, false}}}, {"\\L", {}}}},
      {"F",
       {{"(", {{"(", true}, {E, false}, {")", true}}}, {"id", {{"id", true}}}}},
      {"T",
       {{"(", {{F, false}, {T_dash, false}}},
        {"id", {{F, false}, {T_dash, false}}}}},
      {"T\'",
       {{"*", {{"*", true}, {F, false}, {T_dash, false}}}, {"\\L", {}}}}};
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
  const FollowMap expected_follow = {{"E", {"$", ")"}},
                                     {"E\'", {"$", ")"}},
                                     {"F", {"*", "+", "$", ")"}},
                                     {"T", {"+", "$", ")"}},
                                     {"T\'", {"+", ")", "$"}}};
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
      {{"E", "$"}, {{"#", true}}},
      {{"E", "("}, {{T, false}, {E_dash, false}}},
      {{"E", ")"}, {{"#", true}}},
      {{"E", "id"}, {{T, false}, {E_dash, false}}},

      {{"E'", "$"}, {}},
      {{"E'", ")"}, {}},
      {{"E'", "+"}, {{"+", true}, {T, false}, {E_dash, false}}},

      {{"T", "$"}, {{"#", true}}},
      {{"T", "("}, {{F, false}, {T_dash, false}}},
      {{"T", ")"}, {{"#", true}}},
      {{"T", "+"}, {{"#", true}}},
      {{"T", "id"}, {{F, false}, {T_dash, false}}},

      {{"T'", "$"}, {}},
      {{"T'", ")"}, {}},
      {{"T'", "*"}, {{"*", true}, {F, false}, {T_dash, false}}},
      {{"T'", "+"}, {}},

      {{"F", "$"}, {{"#", true}}},
      {{"F", "("}, {{"(", true}, {"E", false}, {")", true}}},
      {{"F", ")"}, {{"#", true}}},
      {{"F", "*"}, {{"#", true}}},
      {{"F", "+"}, {{"#", true}}},
      {{"F", "id"}, {{"id", true}}}};

  ASSERT_EQ(table, expectedTable);
}
