#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <string>
#include "Parse_Table_Generator.h"
int main() {
    map<string,vector<vector<pair<string ,bool>>>>m;
    map<basic_string<char>, vector<pair<basic_string<char>, vector<pair<basic_string<char>, bool>>>>>first;
    map<string,vector<string>>follow;
    string E="E";
    string T = "T";
    string T_dash = "T'";
    string E_dash = "E'";
    string eps = "\\L";
    string F ="F";
    string F_dash = "F'";
    string P = "P";
    // 3 in sheet
     m.insert({E,
              {{{T, false},{E_dash,false}}}});
    m.insert({E_dash,{{{"+", true},{E_dash,false}},{{eps, true}}}});
    m.insert({T,{{{F, false},{T_dash,false}}}});
    m.insert({T_dash,{{{T, false}},{{eps, true}}}});
    m.insert({F,{{{P, false},{F_dash,false}}}});
    m.insert({F_dash,{{{"*", true},{F, false}},{{eps, true}}}});
    m.insert({P,{{{"(", true},{E,false},{")", true}},{{"a", true}},{{"b", true}},{{"Em", true}}}});

    /*for (auto it = m.rbegin(); it != m.rend(); it++) {
        cout<<it->first<<endl;
    }
    */
    /*m.insert({E,
              {{{T, false},{E_dash,false}}}});
    m.insert({E_dash,{{{"+", true},{T,false},{E_dash,false}},{}}});
    m.insert({T,{{{F, false},{T_dash,false}}}});
    m.insert({T_dash,{{{"*", true},{F, false},{T_dash, false}},{}}});
    m.insert({F,{{{"(", true},{E,false},{")", true}},{{"id", true}}}});
    string  A="A";
    string B ="B";
    string C="C";
    m.insert({A,{{{B, false}},{{C, false},{"x", true}},{{eps, true}}}});
    m.insert({B,{{{C, false}},{{"y", true},{A, false}}}});
    m.insert({C,{{{B, false}},{{"w", true}},{{"z", true}}}});
*/
    Parse_Table_Generator::getFirst(m,first);
    map<basic_string<char>, vector<pair<basic_string<char>, vector<pair<basic_string<char>, bool>>>>>::iterator itr;
    for(itr=first.begin();itr!=first.end();itr++)
    {
        cout<<itr->first<<" ";
        for(int j=0;j<itr->second.size();j++){
            cout<<itr->second[j].first<<" ";
            /*for(auto&x:itr->second[j].second){
                cout<<x.first<<" ";
            }
            cout<<"                  ";*/
        }

        cout<<endl;
    }


    Parse_Table_Generator::getFollow(m,follow,"E");
    //cout<<first.size()<<endl;
    map<string,vector<string>>::iterator it2;
    for(it2=follow.begin();it2!=follow.end();it2++)
    {
        cout<<it2->first<<" ";
        for(int j=0;j<it2->second.size();j++){
            cout<<it2->second[j]<<" ";
        }
        cout<<endl;
    }
    map<pair<string/*NT*/,string/*token*/>,vector<pair<string/*NT or Terminal*/,bool>>> t = Parse_Table_Generator::getTable(m,"E");
    map<pair<string/*NT*/,string/*token*/>,vector<pair<string/*NT or Terminal*/,bool>>>::iterator itr2;
    cout<<"Table"<<endl;
    for(itr2=t.begin();itr2!=t.end();itr2++)
    {
        cout<<itr2->first.first<<" "<<itr2->first.second<<"          ";
        for(auto&vec:itr2->second){
            cout<<vec.first<<" "<<vec.second<<"            ";
        }
        cout<<endl;
    }
    cout<<"Ended"<<endl;
    return 0;
}
