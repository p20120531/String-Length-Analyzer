#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <assert.h>
using namespace std;

class Mgr;
class PT;
class PTNode;
class DG;
class DGNode;

enum Type {
    VAR_INT, VAR_BOOL, VAR_STRING,
    CONST_INT, CONST_BOOL, CONST_STRING,
    BOOL_NOT, BOOL_EQ, BOOL_AND, BOOL_OR, BOOL_ITE,
    INT_PLUS, INT_NEG,
    AUT_LEAF, AUT_CONCATE, AUT_UNION, AUT_INTER, AUT_REPLACE, AUT_COMPLE
    //OP_STRCONCATE, OP_STRLEN, OP_STRINRE, OP_STRREPLACE, OP_STRTORE,
    //OP_RECONCATE, OP_REUNION, OP_REINTER
};

typedef vector<PT*>           PTList;
typedef vector<PTNode*>       PTNodeList;
typedef vector<DG*>           DGList;
typedef vector<DGNode*>       DGNodeList;
typedef pair<string,Type>     Str2Var;
typedef map<string,Type>      Str2VarMap;
typedef pair<string,DGNode*>  Str2DGNode;
typedef map<string,DGNode*>   Str2DGNodeMap;
typedef queue<PTNode*>        PTNodeQueue;

#endif
