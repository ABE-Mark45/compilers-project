#include "Simulator/ParserSimulator.h"
#include <optional>
#include <string>
#include <vector>


ParserSimulator::ParserSimulator(map<pair<NT,string>,ProductionContent> table, ProductionToken topNT) {
  this->table = table;
  tokenStack.push_back({END_INPUT_TOKEN, true});
  tokenStack.push_back(topNT);
}

void ParserSimulator::consumeToken(string token) {
  NT stackToken = tokenStack.back().first;
  
  history.push_back(tokenStack);
  printTokens(tokenStack); cout << '\t';

  if(isTerminalToken(tokenStack.back())) {
    if(token == stackToken){
      tokenStack.pop_back();
      cout << endl;
    } else {
      // TODO: handle error
      string err = "Error: missing token \"" + stackToken + "\", inserted.";
      cout << err << endl;;
      errors.push_back(err);
      tokenStack.pop_back();
      consumeToken(token);
    }
  } else {
    if(table.find({stackToken, token}) != table.end()) {
      ProductionContent pc = table[{stackToken, token}];

      if(isٍٍٍSyncProduction(pc)) {
        tokenStack.pop_back();
        cout <<"sync production\n";
      } 

      else {
        reverse(pc.begin(), pc.end());
        tokenStack.pop_back();
        tokenStack.insert(tokenStack.end(), pc.begin() , pc.end());

        reverse(pc.begin(), pc.end());
        printProduction(stackToken, pc);
        cout << '\n';  
      }

      consumeToken(token);
    } else {
      //TODO: handle error
      string err = "Error:(illegal \""+ stackToken +"\") – discard " + token;
      cout << err << endl;
      errors.push_back(err);

    }
  }
}


// get history of the parsing process
vector<vector<ProductionToken>> ParserSimulator::getHistory() {
  return history;
}
  
bool ParserSimulator::isTerminalToken(ProductionToken pt){
  return pt.second;
}

bool ParserSimulator::isٍٍٍSyncProduction(ProductionContent pt) {
  return pt.size() == 1 && pt.back().first == "#" && pt.back().second;
}

void ParserSimulator::printTokens(vector<ProductionToken> v){ 
  for(ProductionToken pt: v) {
    cout<< pt.first;
  }
}

void ParserSimulator::printProduction(NT nt, ProductionContent pc){ 
  cout << nt <<"-->"; 
  printTokens(pc);
}