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
      R"(IF='if' '(' EXPRESSION ')' '{' STATEMENT '}' 'else' '{' STATEMENT '}')",
      "WHILE='while' '(' EXPRESSION ')' '{' STATEMENT '}'",
      "ASSIGNMENT='id' '=' EXPRESSION ';'",
      "EXPRESSION=SIMPLE_EXPRESSION| \nSIMPLE_EXPRESSION 'relop' "
      "SIMPLE_EXPRESSION",
      "SIMPLE_EXPRESSION=TERM|SIGN TERM|SIMPLE_EXPRESSION 'addop' TERM",
      "TERM=FACTOR|TERM 'mulop' FACTOR",
      "FACTOR='id'|'num'|'(' EXPRESSION ')'",
      "SIGN='+'| '-'"};

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
         {"}", true}}}},
      {"WHILE",
       {{{"while", true},
         {"(", true},
         {"EXPRESSION", false},
         {")", true},
         {"{", true},
         {"STATEMENT", false},
         {"}", true}}}},
      {"ASSIGNMENT",
       {{{"id", true}, {"=", true}, {"EXPRESSION", false}, {";", true}}}},
      {"EXPRESSION",
       {{{"SIMPLE_EXPRESSION", false}},
        {{"SIMPLE_EXPRESSION", false},
         {"relop", true},
         {"SIMPLE_EXPRESSION", false}}}},
      {"SIMPLE_EXPRESSION",
       {{{"TERM", false}},
        {{"SIGN", false}, {"TERM", false}},
        {{"SIMPLE_EXPRESSION", false}, {"addop", true}, {"TERM", false}}}},
      {"TERM",
       {{{"FACTOR", false}},
        {{"TERM", false}, {"mulop", true}, {"FACTOR", false}}}},
      {"FACTOR",
       {{{"id", true}},
        {{"num", true}},
        {{"(", true}, {"EXPRESSION", false}, {")", true}}}},
      {"SIGN", {{{"+", true}}, {{"-", true}}}}};

  CFGParser cfgParser;

  for (const auto line : cfgLines) {
    cfgParser.parseLine(line);
  }

  ASSERT_EQ(trueTable, cfgParser.getProductionsTable());
}