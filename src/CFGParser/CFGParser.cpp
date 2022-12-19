#include <CFGParser/CFGParser.h>
#include <ranges>

void CFGParser::parseLine(std::string_view line) {
  std::string nonTerminalName;
  size_t idx = 0;
  while (idx < line.length() && line.at(idx) != '=') {
    if (!std::isspace(line[idx])) {
      nonTerminalName.push_back(line.at(idx));
    }
    idx++;
  }

  if (idx == line.length()) {
    throw std::runtime_error("Invalid line");
  }

  parseProduction(nonTerminalName, line.substr(idx + 1));
}

/* static */ std::string_view CFGParser::trimString(const std::string_view s) {
  constexpr auto whitespaceCharacters = " \r\t\n";
  auto l = s.find_first_not_of(whitespaceCharacters);
  auto r = s.find_last_not_of(whitespaceCharacters);

  if (l == std::string::npos || r == std::string::npos) {
    return std::string_view{};
  }

  return s.substr(l, r - l + 1);
}

void CFGParser::parseProduction(const std::string& nonTerminalName,
                                std::string_view productionsBody) {

  auto rangeToStringView = [](auto strRange) {
    return std::string_view(&*strRange.begin(),
                            std::ranges::distance(strRange));
  };

  for (const auto production : productionsBody | std::views::split('|') |
                                   std::views::transform(rangeToStringView)) {

    ProductionContent content;
    auto cfgTokens = production | std::views::split(' ') |
                     std::views::transform(rangeToStringView) |
                     std::views::transform(trimString) |
                     std::views::filter([](auto sv) { return !sv.empty(); });

    for (auto token : cfgTokens) {
      if (token.front() == '\'' && token.back() == '\'') {
        auto terminalTokenName = token.substr(1, token.length() - 2);
        content.emplace_back(terminalTokenName, true);
      } else if (token == "\\L") {
        std::string terminalTokenName{token.begin(), token.end()};
        content.emplace_back(terminalTokenName, true);
      } else {
        std::string nonTerminalTokenName{token.begin(), token.end()};
        content.emplace_back(nonTerminalTokenName, false);
      }
    }

    table_[nonTerminalName].emplace_back(std::move(content));
  }
}
