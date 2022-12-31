#include <Simulator/Simulator.h>
#include <optional>
#include <string>
#include <vector>

Simulator::Simulator(std::shared_ptr<const dfa::State> startState,
                     std::ostream& o)
    : startState_(startState),
      currentState_(startState),
      o_(o),
      lastAcceptingState_(nullptr) {}

void Simulator::consumeCharacter(char character) {
  if (std::isspace(character) && currentState_ == startState_) {
    return;
  }
  buffer.push_back(character);
  auto nextState = currentState_->moveThrough(character);
  if (nextState) {
    currentState_ = nextState;
    // save last accepting value
    if (currentState_->getAcceptValue()) {
      lastAcceptingState_ = currentState_;
      buffer.clear();
    }
    return;
  } else {
    flushLastAcceptingState();
  }
}

void Simulator::flushLastAcceptingState() {
  if (lastAcceptingState_) {
    // flush last accepting state and consume succeeding characters
    tokens.push_back(lastAcceptingState_->getAcceptValue().value().value);
    lastAcceptingState_.reset();
    currentState_ = startState_;
    std::vector<char> newInput = std::move(buffer);
    for (char c : newInput) {
      consumeCharacter(c);
    }
  } else if (!buffer.empty()) {
    auto errorMessage =
        "No matched token for " + std::string(buffer.begin(), buffer.end());
    o_ << errorMessage << '\n';
    errors.emplace_back(errorMessage);
    buffer.clear();
  }
}

void Simulator::finishSimulation() {
  while (lastAcceptingState_) {
    flushLastAcceptingState();
  }

  // are there characters that couldn't be analyzed?
  if (!buffer.empty()) {
    auto errorMessage =
        "No matched token for " + std::string(buffer.begin(), buffer.end());
    o_ << errorMessage << '\n';
    errors.emplace_back(errorMessage);
    buffer.clear();
  }
}

std::vector<std::string> Simulator::getErrors() {
  return vector<string>(errors);
}

std::vector<std::string> Simulator::getTokens() {
  return vector<string>(tokens);
}

ParserSimulator::ParserSimulator(map<pair<NT,string>,ProductionContent> table, ProductionToken topNT) {
  tokenStack.push_back({END_INPUT_TOKEN, true});
  tokenStack.push_back(topNT);
}

void ParserSimulator::consumeToken(string token) {
  NT nt = tokenStack.back().first;
  
  history.push_back(tokenStack);
  printTokens(tokenStack); cout << '\t';

  if(isTerminalToken(tokenStack.back())) {
    if(token == nt){
      tokenStack.pop_back();
    } else {
      // TODO: handle error
      string err = "Error: missing token \"" + token + "\", inserted.";
      cout << err << endl;;
      errors.push_back(err);
      tokenStack.pop_back();
    }
  } else {
    if(table.find({nt, token}) != table.end()) {
      ProductionContent pc = table[{nt, token}];

      if(isٍٍٍSyncProduction(pc)) {
        tokenStack.pop_back();
        cout <<"sync production\n";
      } 

      else {
        reverse(pc.begin(), pc.end());
        tokenStack.pop_back();
        tokenStack.insert(tokenStack.end(), pc.begin() , pc.end());

        reverse(pc.begin(), pc.end());
        printProduction(nt, pc);
        cout << '\n';  
      }

      consumeToken(token);
    } else {
      //TODO: handle error
      string err = "Error:(illegal \""+ nt +"\") – discard " + token;
      cout << err << endl;
      errors.push_back(err);

    }
  }
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