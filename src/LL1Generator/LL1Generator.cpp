#include "./LL1Generator.h"
#include <set>
#include <iostream>


void LL1Generator::removeEpsilonProductions(){
    //TODO
}

void LL1Generator::removeCycles(){
    //TODO
}

void LL1Generator::eliminateImmediateLeftRecursion(std::string nonTerminalName){
    // split the productions to two groups, those that start with NT A and those that don't
    // create new NT A'
    // for each production that doesn't start with A
    //      insert concat(production, A') to productions of A
    // for each production that starts with A
    //      insert concat(prodcution[1:], A') to prodcutions of A'
    std::vector<ProductionContent> start_with_A, not_start_with_A;
    for (auto& prod: table_[nonTerminalName]){
        if (prod[0].first == nonTerminalName)
            start_with_A.push_back(prod);
        else
            not_start_with_A.push_back(prod);
    }

    std::vector<ProductionContent> new_A_productions, A_prime_productions;
    std::string A_prime = nonTerminalName+"'";
    for (auto prod: not_start_with_A){
        prod.emplace_back(A_prime, false);
        new_A_productions.push_back(prod);
    }
    for (auto prod: start_with_A){
        prod.erase(prod.begin());
        if (prod.size() == 0) //will become A' -> A'!
            continue;
        prod.emplace_back(A_prime, false);
        A_prime_productions.push_back(prod);
    }
    A_prime_productions.push_back({}); //epsilon production

    table_[nonTerminalName] = std::move(new_A_productions);
    table_[A_prime] = std::move(A_prime_productions);
}


LL1Generator::LL1Generator(ProductionsTable& table_in): table_(table_in){
    removeEpsilonProductions();
    removeCycles();

    // for each NT Ai
    //      obtain the set of previous NTs
    //      for each production of Ai
    //          if the production starts with a previous NT Aj
    //              replace it with the new productions
    //          else copy it as is
    
    std::vector<std::string> NTs;
    for (auto& [key, val]: table_)
        NTs.push_back(key);

    for (int i = 0; i < NTs.size(); i++){
        auto& Ai = NTs[i];
        std::vector<ProductionContent> new_productions;

        std::set<ProductionToken> prev_NTs;
        for (auto j = 0; j < i; j++)
            prev_NTs.insert({NTs[j], false});

        for (auto& Ai_prod: table_[Ai]){
            if (prev_NTs.contains(Ai_prod[0])){
                auto Aj = Ai_prod[0].first;
                // replacement:
                // for each production p of Aj
                //      insert concat(p, Ai_prod[1:]) to the new Ai productions
                for (auto& Aj_prod: table_[Aj]){
                    ProductionContent content;
                    content.insert(content.end(), Aj_prod.begin(), Aj_prod.end());
                    content.insert(content.end(), Ai_prod.begin() + 1, Ai_prod.end());
                    new_productions.push_back(content);
                }
            }
            else {
                new_productions.push_back(Ai_prod);
            }
        }
        table_[Ai] = std::move(new_productions);

        eliminateImmediateLeftRecursion(Ai);
    }
}
