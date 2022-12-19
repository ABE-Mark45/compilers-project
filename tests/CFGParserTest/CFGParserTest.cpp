#include <CFGParser/CFGParser.h>
#include <gtest/gtest.h>
#include <string>
#include <vector>

TEST(CFGParserTest, Example) {
  const std::vector<std::string_view> cfgLines{
      "METHOD_BODY = STATEMENT_LIST",
      "STATEMENT_LIST=STATEMENT|STATEMENT_LIST STATEMENT",
      "STATEMENT = DECLARATION\n"
      "| IF \n"
      " | WHILE\n "
      "| ASSIGNMENT ",
      R"(DECLARATION =PRIMITIVE_TYPE 'id' ';')",
      R"(IF='if' '(' EXPRESSION ')' '{' STATEMENT '}' 'else' '{' STATEMENT '}')"};

  const ProductionsTable trueTable{
      {"METHOD_BODY", {{{"STATEMENT_LIST", false}}}},
      {"STATEMENT_LIST",
       {
           {{"STATEMENT", false}},
           {{"STATEMENT_LIST", false}, {"STATEMENT", false}},
       }},
      {"STATEMENT",
       {{{"DECLARATION", false}},
        {{"IF", false}},
        {{"WHILE", false}},
        {{"ASSIGNMENT", false}}}},
      {"DECLARATION", {{{"PRIMITIVE_TYPE", false}, {"id", true}, {";", true}}}},
      {"IF",
       {{{"if", true},
         {"(", true},
         {"EXPRESSION", false},
         {")", true},
         {"{", true},
         {"STATEMENT", false},
         {"}", true},
         {"else", true},
         {"{", true},
         {"STATEMENT", false},
         {"}", true}}}}};

  CFGParser cfgParser;

  for (const auto line : cfgLines) {
    cfgParser.parseLine(line);
  }

  ASSERT_EQ(trueTable, cfgParser.getProductionsTable());
}