#include <Readers/LanguageRulesReader.h>

LanguageRulesReader::LanguageRulesReader(
    const std::filesystem::path& rulesFilePath)
    : inputFile_(rulesFilePath) {
  if (!inputFile_.is_open()) {
    throw std::runtime_error("could not open file " + rulesFilePath.string());
  }
}

std::optional<std::string> LanguageRulesReader::getLine() {
  std::string str;
  if (std::getline(inputFile_, str)) {
    return str;
  }
  return std::nullopt;
}