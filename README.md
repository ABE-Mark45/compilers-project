# compilers-project


**MODULES**

GrammarParser: 
	Input: file
	Output: Map<string, vector<vector<pair<string, boolean>>>> 

LLGenerator: 
	Input:Map
	Output Map

TableGenerator: 
	Input: Map<string, vector<vector<pair<string, boolean>>>>
	Output: Map<pair<NT, Tokens of lexer>, &vector<T or NT>> --> pointers or indices or references, whatever works. Indexed as map[{A, token}]

Simulator: 
	Input: Map<pair<NT, Tokens of lexer>, &vector<T or NT>> && output stream of lexer
	Output: the stack at each parsing step
