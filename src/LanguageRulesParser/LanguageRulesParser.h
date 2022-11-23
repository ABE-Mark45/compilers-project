#pragma once
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <iostream>

class LanguageRulesParser {
public:
    // Interface to parsing logic
    void parseLine(const std::string& line);

    // Debugging function that outputs all parsed tokens
    void show();


private:
    // Parse a definition line on the form <identifier>=<regex>
    void parseDef(const std::string& identifier, const std::string& line, int idx);

    // A helper function to control whether to parse a regex definition or expression
    void parseDefOrExp(const std::string& line);
    
    std::unordered_map<std::string, std::vector<std::string>> regexDefinitions;
};