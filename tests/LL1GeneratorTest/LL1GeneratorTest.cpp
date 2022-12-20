#include <LL1Generator/LL1Generator.h>
#include <gtest/gtest.h>
#include <string>
#include <vector>

TEST(LL1GeneratorTest, BigTest) {

  ProductionsTable inputTable{
      {"S", {{{"A", false}, {"a", true}},{{"b", true}}}},
      {"A",
       {
           {{"A", false}, {"c", true}},
           {{"S", false}, {"d", true}},
           {{"l", true}}
       }}};

  // The map orders the non-terminals by alphabetic order
  const ProductionsTable outputTable{
    {"S", 
    {
        {{"l", true}, {"A'", false}, {"a", true}, {"S'", false}},
        {{"b", true}, {"S'", false}}
    }},
    {"S'",
    {
        {{"d", true}, {"A'", false}, {"a", true}, {"S'", false}},
        {}
    }},
    {"A",
    {
        {{"S", false}, {"d", true}, {"A'", false}},
        {{"l", true}, {"A'", false}}
    }},
    {"A'",
    {
        {{"c", true}, {"A'", false}},
        {}
    }}
  };

  LL1Generator ll1Generator(inputTable);

  ASSERT_EQ(outputTable, ll1Generator.getProductionsTable());
}


TEST(LL1GeneratorTest, SmallTest) {

  ProductionsTable inputTable{
      {"A",
       {
           {{"A", false}},
           {{"b", true}}
       }}};

  // Epsilon productions are empty vectors
  const ProductionsTable outputTable{
    {"A", 
    {
        {{"b", true}, {"A'", false}}
    }},
    {"A'",
    {
        {}
    }}
  };

  LL1Generator ll1Generator(inputTable);

  ASSERT_EQ(outputTable, ll1Generator.getProductionsTable());
}