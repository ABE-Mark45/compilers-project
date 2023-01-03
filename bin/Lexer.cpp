#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "DFABuilder/DFABuilder.h"
#include "LanguageRulesParser/LanguageRulesParser.h"
#include "NFABuilder/NFABuilder.h"
#include "Readers/LanguageRulesReader.h"
#include "Readers/ProgramReader.h"
#include "Simulator/LexicalSimulator.h"
#include "utils/DFAPrinter.h"
#include "LL1Generator/LL1Generator.h"
#include "Simulator/ParserSimulator.h"
#include "ParseTableGenerator/ParseTableGenerator.h"
#include "Readers/CFGReader.h"

// print productionTable
void printProductionTable(ProductionsTable pt){
  cout << "Production Table:" << endl;
  for(auto& [nt, productions]: pt){
    cout << nt << " -> ";
    for(auto& production: productions){
      for(auto& token: production){
        cout << token.first << " ";
      }
      cout << "| ";
    }
    cout << endl;
  }
  cout << " -------------- "<< endl;
}

// print vector of strings to file
void printVectorToFile(std::ofstream& file, std::vector<std::string> v){
  for(auto& s: v){
    file << s << endl;
  }
}

auto main(int argc, char** argv) -> int {
  if (argc != 6) {
    throw std::runtime_error(
        "Lexer <rules_path> <program_path> <grammar_path> <table_out_path> <tokens_out_path>");
  }
  const std::filesystem::path rulesFilePath{argv[1]};
  LanguageRulesReader rulesReader(rulesFilePath);

  const std::filesystem::path programFilePath{argv[2]};
  ProgramReader programReader(programFilePath);

  const std::filesystem::path grammarFilePath{argv[3]};
  CFGReader grammarReader(grammarFilePath);

  const std::filesystem::path tableOutputPath{argv[4]};
  std::ofstream tableOutputFile(tableOutputPath,
                                std::ios::out | std::ios::trunc);

  const std::filesystem::path tokensOutputPath{argv[5]};
  std::ofstream tokensOutputFile(tokensOutputPath,
                                 std::ios::out | std::ios::trunc);
  // construct lexical parser
  LanguageRulesParser parser;
  while (auto rule = rulesReader.getLine()) {
    parser.parseLine(rule.value());
  }

  nfa::NFABuilder nfaBuilder(
      parser.getPostfixRegexExpressions(), parser.getKeywords(),
      parser.getPunctuationCharacters(), parser.getPriorities());

  auto nfa = nfaBuilder.getCombinedNFA();
  
  auto dfaStartState =  DFABuilder::minimizeDFA(DFABuilder::buildDFA(std::move(nfa)));

  // lexical analysis
  LexicalSimulator s(dfaStartState);
  while (programReader.hasChar()) {
    s.consumeCharacter(programReader.getChar());
  }
  s.finishSimulation();

  // lexical analysis errors
  if(s.getErrors().size()) {
    std::cout << "found the following errors while doing lexical analysis:\n";
    for(auto& e: s.getErrors())
      std::cout << e << "\n";
  }
  vector<string> lexicalTokens = s.getTokens();
  printVectorToFile(tokensOutputFile, lexicalTokens);
  
  // print lexical parser DFA
  printDFA(dfaStartState, tableOutputFile);

  cout << "starting CFG parsing .. \n";

  // construct CFG parser production table
  CFGParser cfgParser;
  string line = "";
  while (auto rule = grammarReader.getLine()) {
    cfgParser.parseLine(rule.value());
  }
  ProductionsTable pt = cfgParser.getProductionsTable();

  printProductionTable(pt);

  // left factor & .. etc
  LL1Generator llg(pt);
  ProductionsTable pt_modified = llg.getProductionsTable();    

  printProductionTable(pt_modified);

  // get parse table 
  ProductionToken topNT = {"METHOD_BODY", false};
  const ParseTable parseTable = ParseTableGenerator::getTable(pt_modified, topNT.first);
  
  ParserSimulator parserSimulator(parseTable, topNT);
  // simulate the CFG stack parsing
  for(string token: lexicalTokens) {
    parserSimulator.consumeToken(token);
  }
  parserSimulator.consumeToken("$");
  return 0;
}

