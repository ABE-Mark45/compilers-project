#include "./LL1Generator.h"
#include <set>
#include <unordered_set>
#include <iostream>
#include <algorithm>


void LL1Generator::removeEpsilonProductions(){
    // while some non-terminals still have epsilon productions
    //      for each non terminal:
    //          if it has an epsilon production, mark it and remove the production
    //      for each non terminal:
    //          for each production:
    //              for each NT in production (including the productions that are added in the meantime):
    //                  if NT has an epsilon production
    //                      add a production without it
    //      if no epsilon production could be eliminated in this pass
    //          break because the language includes the empty string

    while(true){
        bool removed_epsilon = false;
        std::unordered_set<std::string> has_epsilon;
        for (auto& [key, val]: table_){
            for (auto it = val.begin(); it < val.end(); it++){
                auto p = *it;
                if (p.empty()){
                    has_epsilon.insert(key);
                    val.erase(it);
                    break;
                }
            }
        }

        if (has_epsilon.empty()) // Done!
            break;

        for (auto& [key, val]: table_){
            for (int i = 0; i < (int)val.size(); i++){
                auto prod = val[i];
                for (int j = 0; j < (int)prod.size(); j++){
                    auto term = prod[j];
                    if (!term.second && has_epsilon.contains(term.first)){
                        removed_epsilon = true;
                        ProductionContent new_production;
                        for (int k = 0; k < (int)prod.size(); k++)
                            if (k != j)
                                new_production.emplace_back(prod[k]);
                        val.emplace_back(new_production);
                    }
                }
            }
        }

        if (!removed_epsilon){
            std::cout<<"Warning: The language includes epsilon";
            break;
        }
    }
}

void LL1Generator::removeCycles(){
    //TODO
}

void LL1Generator::eliminateImmediateLeftRecursion(const std::string& nonTerminalName){
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

    if (start_with_A.empty()) // no left recursion
        return;

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

void LL1Generator::eliminateLeftRecursion(){
    // *Eliminate left recursion*:
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

int LL1Generator::lengthOfCommonPrefix(const ProductionContent& p1, const ProductionContent& p2){
    if (p1.empty())
        return 0;
    int ans = 0;
    while(p1[ans] == p2[ans]) ans++;
    return ans;
}

void LL1Generator::leftFactor(){
    // *Left factor*:
    // for each NT A in table:
    //      sort productions
    //      find length of common prefix between each 2 consecutive productions
    //      factors = 0
    //      do{
    //          find a run of productions with a longest common prefix (from, to, len)
    //          if (len == 0) break
    //          create NT A"#factors with productions = productions[A][from:to][len:]
    //          replace productions[A][from:to] with prefix + A"#factors
    //          remove common[A][from:to - 1]
    //      } while there is a non-empty common prefix between any two productions 
    
    std::vector<std::string> NTs;
    for (auto& [NT, productions]: table_)
        NTs.push_back(NT);

    for (auto& A: NTs){
        std::vector<ProductionContent> productions(table_[A].size());
        std::copy(table_[A].begin(), table_[A].end(), productions.begin());

        std::sort(productions.begin(), productions.end());
        int n_A_productions = productions.size();

        int common[n_A_productions - 1];
        for (int i = 0; i < n_A_productions - 1; i++) 
            common[i] = lengthOfCommonPrefix(productions[i], productions[i+1]);
        
        int from, to, prefix_len, n_factors = 0;
        bool run = false;
        while (true){

            from = 0; to = -1; prefix_len = 0;
            for (int i = 0; i < n_A_productions - 1; i++){
                if (common[i] > prefix_len){
                    prefix_len = common[i];
                    from = i;
                    to = i + 2;
                    run = true;
                }else if (common[i] == prefix_len){
                    if (run) to++;
                }else if (common[i] < prefix_len){
                    run = false;
                }
            }

            if (prefix_len == 0) break;

            ProductionContent prefix(productions[from].begin(),
                                     productions[from].begin() + prefix_len); 
            

            std::string A_prime = A + '#' + std::to_string(n_factors);
            std::vector<ProductionContent> A_prime_productions;
            for (int i = from; i < to; i++){
                ProductionContent production(productions[i].begin() + prefix_len,
                                             productions[i].end());
                A_prime_productions.emplace_back(production);
            }
            table_[A_prime] = std::move(A_prime_productions);

            prefix.emplace_back(A_prime, false);
            productions[from] = std::move(prefix);
            productions.erase(productions.begin() + from + 1,
                              productions.begin() + to);
            
            n_A_productions = productions.size();
            for (int i = from; i < n_A_productions - 1; i++)
                common[i] = common[i + to - from - 1];

            n_factors++;
        }

        if (n_factors) table_[A] = std::move(productions);
    }
}

LL1Generator::LL1Generator(ProductionsTable& table_in): table_(table_in){
    removeEpsilonProductions();
    removeCycles();

    // TODO, make changes only if needed
    eliminateLeftRecursion();

    leftFactor();
}
