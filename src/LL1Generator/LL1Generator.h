#pragma once
#include <string>
#include "../CFGParser/CFGParser.h"

class LL1Generator {
 public:
  LL1Generator(ProductionsTable& table_);

  const auto& getProductionsTable() const { return table_; }

 private:
  void removeEpsilonProductions();
  void removeCycles();
  void eliminateImmediateLeftRecursion(std::string nonTerminalName);

  ProductionsTable table_;
};