#include <LL1Generator/LL1Generator.h>
#include <gtest/gtest.h>
#include <string>
#include <vector>

TEST(LL1GeneratorTest, BigTest) {

  ProductionsTable inputTable{
      {"S", {{{"A", false}, {"a", true}}, {{"b", true}}}},
      {"A",
       {{{"A", false}, {"c", true}},
        {{"S", false}, {"d", true}},
        {{"l", true}}}}};

  // The map orders the non-terminals by alphabetic order
  const ProductionsTable outputTable{
      {"S",
       {{{"l", true}, {"A'", false}, {"a", true}, {"S'", false}},
        {{"b", true}, {"S'", false}}}},
      {"S'", {{{"d", true}, {"A'", false}, {"a", true}, {"S'", false}}, {}}},
      {"A",
       {{{"S", false}, {"d", true}, {"A'", false}},
        {{"l", true}, {"A'", false}}}},
      {"A'", {{{"c", true}, {"A'", false}}, {}}}};

  LL1Generator ll1Generator(inputTable);

  ASSERT_EQ(outputTable, ll1Generator.getProductionsTable());
}

TEST(LL1GeneratorTest, SmallTest) {

  ProductionsTable inputTable{{"A", {{{"A", false}}, {{"b", true}}}}};

  // Epsilon productions are empty vectors
  const ProductionsTable outputTable{{"A", {{{"b", true}, {"A'", false}}}},
                                     {"A'", {{}}}};

  LL1Generator ll1Generator(inputTable);

  ASSERT_EQ(outputTable, ll1Generator.getProductionsTable());
}

TEST(LL1GeneratorTest, FactoringSmall) {

  ProductionsTable inputTable{
      {"A",
       {{{"X", false}, {"Y", false}, {"Z", false}, {"c", true}},
        {{"X", false}, {"Y", false}, {"Z", false}, {"b", true}}}}};

  // Epsilon productions are empty vectors
  const ProductionsTable outputTable{
      {"A", {{{"X", false}, {"Y", false}, {"Z", false}, {"A#0", false}}}},
      {"A#0", {{{"b", true}}, {{"c", true}}}}};

  LL1Generator ll1Generator(inputTable);

  ASSERT_EQ(outputTable, ll1Generator.getProductionsTable());
}

TEST(LL1GeneratorTest, FactoringBig) {

  ProductionsTable inputTable{
      {"A",
       {{{"X", false}, {"Y", false}, {"f", true}},
        {{"X", false}, {"Y", false}, {"g", true}},
        {{"X", false}, {"Y", false}, {"Z", false}, {"c", true}},
        {{"X", false}, {"Y", false}, {"Z", false}, {"b", true}}}},
      {"B", {{{"b", true}}}}};

  // Epsilon productions are empty vectors
  const ProductionsTable outputTable{
      {"A", {{{"X", false}, {"Y", false}, {"A#1", false}}}},
      {"A#0", {{{"b", true}}, {{"c", true}}}},
      {"A#1", {{{"Z", false}, {"A#0", false}}, {{"f", true}}, {{"g", true}}}},
      {"B", {{{"b", true}}}}};

  LL1Generator ll1Generator(inputTable);

  ASSERT_EQ(outputTable, ll1Generator.getProductionsTable());
}

TEST(LL1GeneratorTest, AlreadyLL1) {

  ProductionsTable inputTable{{"A", {{{"a", true}, {"B", false}}}},
                              {"B",
                               {
                                   {{"b", true}, {"C", false}, {"b", true}}  //,
                                   //{}
                               }},
                              {"C",
                               {
                                   {{"c", false}}  //,
                                                   //{}
                               }}};

  // Epsilon productions are empty vectors
  const ProductionsTable outputTable{
      {"A", {{{"a", true}, {"B", false}}}},
      {"B",
       {
           {{"b", true}, {"C", false}, {"b", true}}  //,
                                                     //{}
       }},
      {"C",
       {
           {{"c", false}}  //,
                           //{}
       }}};

  LL1Generator ll1Generator(inputTable);

  ASSERT_EQ(outputTable, ll1Generator.getProductionsTable());
}

TEST(LL1GeneratorTest, RemoveEpsilon) {

  ProductionsTable inputTable{
      {"A", {{{"a", true}, {"B", false}}}},
      {"B", {{{"b", true}, {"C", false}, {"b", true}}, {}}},
      {"C", {{{"c", false}}, {}}}};

  // Epsilon productions are empty vectors
  const ProductionsTable outputTable{
      {"A", {{{"a", true}, {"A#0", false}}}},
      {"A#0", {{}, {{"B", false}}}},
      {"B", {{{"b", true}, {"B#0", false}}}},
      {"B#0", {{{"C", false}, {"b", true}}, {{"b", true}}}},
      {"C", {{{"c", false}}}}};

  LL1Generator ll1Generator(inputTable);

  ASSERT_EQ(outputTable, ll1Generator.getProductionsTable());
}

/*
UGHHHH I need to remove redundant productions.. 
TEST(LL1GeneratorTest, RemoveEpsilon2) {

  ProductionsTable inputTable{
      {"A",
       {
        {{"B", false}, {"a", true}, {"B", false}}
       }
      },
      {"B",
       {
        {{"b", true}},
        {}
       }
      },
      {"C",
        {
          {{"B", false}}
        }
      }
    };

  // Epsilon productions are empty vectors
  const ProductionsTable outputTable{
      {"A",
       {
        {{"B", false},{"B", false}},
        {{"B", false}},
        {{"B", false}},
        {} //TODO
       }
      },
      {"B",
       {
        {{"b", true}},
        {}
       }
      }
    };

  LL1Generator ll1Generator(inputTable);

  ASSERT_EQ(outputTable, ll1Generator.getProductionsTable());
}*/