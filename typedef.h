#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
using namespace std;

class Mgr;
class PT;
class PTNode;
class DG;
class DGNode;

enum VarType {
    VAR_INT,
    VAR_BOOL,
    VAR_STRING
};

enum OpType {
    OP_NONE,
    OP_NOT, OP_NEG, OP_EQ, OP_AND, OP_OR, OP_ITE,
    OP_STRCONCATE, OP_STRLEN, OP_STRINRE, OP_STRREPLACE, OP_STRTORE,
    OP_RECONCATE, OP_REUNION, OP_REINTER
};

typedef vector<PT*>           PTList;
typedef vector<PTNode*>       PTNodeList;
typedef vector<DG*>           DGList;
typedef vector<DGNode*>       DGNodeList;
typedef pair<string,VarType>  Str2Var;
typedef map<string,VarType>   Str2VarMap;

#endif
