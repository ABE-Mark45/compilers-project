#pragma once
#include <filesystem>
#include <fstream>

class ProgramReader {
 public:
  explicit ProgramReader(const std::filesystem::path& programPath);
  ProgramReader() = delete;

  bool hasChar() const;
  char getChar();

 private:
  std::ifstream inputFile_;
  std::string currentLine_;
  int currentIdx;
  bool finishedReading_;
};