#pragma once
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using ProductionToken = std::pair<std::string, bool>;

using ProductionContent = std::vector<ProductionToken>;

using ProductionsTable =
    std::unordered_map<std::string, std::vector<ProductionContent>>;

using ParseTable =
    std::map<std::pair<std::string /*NT*/, std::string /*token*/>,
             ProductionContent>;

using FirstSet = std::set<std::pair<std::string, ProductionContent>>;

using FirstMap = std::unordered_map<std::string, FirstSet>;

using FollowSet = std::unordered_set<std::string>;

using FollowMap = std::unordered_map<std::string, FollowSet>;