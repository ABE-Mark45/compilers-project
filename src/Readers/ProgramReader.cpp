#include <Readers/ProgramReader.h>

ProgramReader::ProgramReader(const std::filesystem::path& programPath)
    : inputFile_(programPath), currentIdx(0), finishedReading_(false) {
  std::getline(inputFile_, currentLine_);
  currentLine_.push_back('\n');
}

bool ProgramReader::hasChar() const {
  return !finishedReading_;
}
char ProgramReader::getChar() {
  if (finishedReading_) {
    throw std::runtime_error("No more characters to read");
  }
  char c = currentLine_[currentIdx++];

  if (currentIdx == currentLine_.size()) {
    if (std::getline(inputFile_, currentLine_)) {
      currentIdx = 0;
      currentLine_.push_back('\n');
    } else {
      finishedReading_ = true;
      inputFile_.close();
    }
  }
  return c;
}
