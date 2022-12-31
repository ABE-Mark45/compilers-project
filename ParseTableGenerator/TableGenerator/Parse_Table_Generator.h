#include <string>
#include <map>
#include <vector>
#include <utility>
using namespace std;
/*
#ifndef TABLEGENERATOR_PARSE_TABLE_GENERATOR_H
#define TABLEGENERATOR_PARSE_TABLE_GENERATOR_H

/*
 * Input: Map<string, vector<vector<pair<string, boolean>>>>
	Output: Map<pair<NT, Tokens of lexer>, &vector<T or NT>> --> pointers or indices or references,
 whatever works. Indexed as map[{A, token}]
 * */
class Parse_Table_Generator {
public:
    static map<pair<string/*NT*/, string/*token*/>, vector<pair<string/*NT or Terminal*/, bool>>>
    getTable(const map<string, vector<vector<pair<string, bool>>>> &,const string& start_symbol);

    //this string means state , every state maps to more than one production
    //every production it maps to is either true or false
    static void getFirst(const map<string, vector<vector<pair<string, bool>>>> &m, map<basic_string<char>, vector<pair<basic_string<char>, vector<pair<basic_string<char>, bool>>>>> &first);
    static void getFollow(const map<string, vector<vector<pair<string, bool>>>> &m, map<string, vector<string>> &follow,const string& start_symbol);
    static bool get_is_ambiguous();
};

//#endif //TABLEGENERATOR_PARSE_TABLE_GENERATOR_H
