#pragma once
#include <filesystem>
#include <fstream>
#include <optional>
#include <string>

class LanguageRulesReader {
 public:
  explicit LanguageRulesReader(const std::filesystem::path& rulesFilePath);
  LanguageRulesReader() = delete;

  std::optional<std::string> getLine();

 private:
  std::ifstream inputFile_;
};