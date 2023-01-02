#pragma once
#include <utils/CFGTypes.h>
#include <string>
#include <unordered_map>
#include <vector>

class CFGParser {
 public:
  void parseLine(std::string_view line);

  const auto& getProductionsTable() const { return table_; }

 private:
  static std::string_view trimString(const std::string_view s);

  void parseProduction(const std::string& nonTerminalName,
                       std::string_view productionsBody);

  ProductionsTable table_;
};