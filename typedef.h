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
#include <stdlib.h>
#include "utility.h"
using namespace std;

class Mgr;
class PT;
class PTNode;
class DG;
class DGNode;
class PredicateNode;

enum Type {
    VAR_INT=0, VAR_BOOL, VAR_STRING,
    CONST_INT, CONST_BOOL, CONST_STRING,
    STRING_LEN,
    // classified by return value
    BOOL_NOT=10, BOOL_EQ, BOOL_NEQ, BOOL_AND, BOOL_OR, BOOL_ITE, BOOL_LT, BOOL_LTOEQ, BOOL_MT, BOOL_MTOEQ=30,
    INT_PLUS=40, INT_MINUS, INT_DIV, INT_POS, INT_NEG=50,
    AUT_CONCATE=60, AUT_UNION, AUT_INTER, AUT_REPLACE, AUT_COMPLE
    //OP_STRCONCATE, OP_STRLEN, OP_STRINRE, OP_STRREPLACE, OP_STRTORE,
    //OP_RECONCATE, OP_REUNION, OP_REINTER
};

enum FType {
    SMT2, VMT
};

typedef vector<PT*>           PTList;
typedef vector<PTNode*>       PTNodeList;
typedef vector<DG*>           DGList;
typedef vector<DGNode*>       DGNodeList;
typedef pair<string,Type>     Str2Type;
typedef map<string,Type>      Str2TypeMap;
typedef pair<string,size_t>   Str2Uint;
typedef map<string,size_t>    Str2UintMap;
typedef pair<string,DGNode*>  Str2DGNode;
typedef map<string,DGNode*>   Str2DGNodeMap;
typedef queue<PTNode*>        PTNodeQueue;
typedef vector<string>        LCList;
typedef vector<vector<string> > LoLCList;
#endif
