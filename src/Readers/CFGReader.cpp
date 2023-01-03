#include "Readers/CFGReader.h"

using namespace std;

CFGReader::CFGReader(
    const std::filesystem::path& rulesFilePath)
    : inputFile_(rulesFilePath) {
        str = "";
  if (!inputFile_.is_open()) {
    throw std::runtime_error("could not open file " + rulesFilePath.string());
  }
}


// trim space from start of the string
string trimStart(string s) {
  int i = 0;
  while (s[i] == ' ') {
    i++;
  }
  return s.substr(i);
}

std::optional<std::string> CFGReader::getLine() {
    std::string line=str;
    std::string line1,line2 = "";

    if(line == ""){
        std::getline(inputFile_, str);
        line += str; 
    } 
    
    str="";
    
    while(std::getline(inputFile_, str)){
        str = trimStart(str);
        if(str[0] != '|'){
            break;
        }
        line += str;
    }

    return line == "" ? std::nullopt : std::optional<std::string>(line);
}
