#ifndef _TYPEDEF_
#define _TYPEDEF_
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
using namespace std;

class DGMgr;
class DG;
class Node;

enum VarType {
    VAR_INT,
    VAR_BOOL,
    VAR_STRING
};

enum OpType {
    OP_NOT, OP_NEG, OP_EQ, OP_AND, OP_OR, OP_ITE,
    OP_STRCONCATE, OP_STRLEN, OP_STRINRE, OP_STRREPLACE,
    OP_RECONCATE, OP_REUNION, OP_REINTER
};

typedef vector<DG*>           DGList;
typedef vector<Node*>         NodeList;
typedef pair<string,VarType>  Str2Var;
typedef map<string,VarType>   Str2VarMap;

#endif
