#include <utils/CFGTypes.h>
#include <map>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
using namespace std;

class ParseTableGenerator {
 public:
  static ParseTable getTable(const ProductionsTable&,
                             const string& start_symbol);

  //this string means state , every state maps to more than one production
  //every production it maps to is either true or false
  static FirstMap getFirstMap(const ProductionsTable& productionsTable);
  static FollowMap getFollowMap(const ProductionsTable& productionsTable,
                                const FirstMap& firstMap,
                                const string& start_symbol);
  static bool get_is_ambiguous();
};
