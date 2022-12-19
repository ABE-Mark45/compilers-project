#pragma once
#include <string>
#include <unordered_map>
#include <vector>

using ProductionToken = std::pair<std::string, bool>;

using ProductionContent = std::vector<ProductionToken>;

using ProductionsTable =
    std::unordered_map<std::string, std::vector<ProductionContent>>;

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