#pragma once
#include <utils/CFGTypes.h>
#include <string>

class LL1Generator {
 public:
  LL1Generator(ProductionsTable& table_);

  const auto& getProductionsTable() const { return table_; }

 private:
  void removeEpsilonProductions();
  void removeCycles();
  void eliminateImmediateLeftRecursion(const std::string& nonTerminalName);
  void eliminateLeftRecursion();
  int lengthOfCommonPrefix(const ProductionContent&, const ProductionContent&);
  void leftFactor();

  ProductionsTable table_;
};