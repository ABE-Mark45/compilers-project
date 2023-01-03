#pragma once
#include <filesystem>
#include <fstream>
#include <optional>
#include <string>

class CFGReader {
 public:
  explicit CFGReader(const std::filesystem::path& rulesFilePath);
  CFGReader() = delete;

  std::optional<std::string> getLine();

 private:
  std::ifstream inputFile_;
  std::string str;
};