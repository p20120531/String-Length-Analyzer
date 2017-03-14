#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_
//#define _NDIR_
//#define _NLOG_

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

class KaluzaMgr;
class PT;
class PTNode;
class DG;
class DGNode;
class PredicateNode;

enum Type {
    VAR_INT=0, VAR_BOOL, VAR_STRING,
    CONST_INT, CONST_BOOL, CONST_STRING,
    // classified by return value
    BOOL_NOT=10, BOOL_EQ, BOOL_NEQ, BOOL_AND, BOOL_OR, BOOL_ITE, BOOL_LT, BOOL_LTOEQ, BOOL_MT, BOOL_MTOEQ, BOOL_STRINRE, BOOL_STRNINRE, 
    INT_PLUS=40, INT_MINUS, INT_DIV, INT_POS, INT_NEG, INT_STRLEN,
    STRING_CONCATE, STRING_REPLACE,
    REGEX_STRTORE, REGEX_CONCATE, REGEX_UNION, REGEX_INTER,
    AUT_CONCATE=60, AUT_UNION, AUT_INTER, AUT_REPLACE, AUT_COMPLE
    //OP_STRCONCATE, OP_STRLEN, OP_STRINRE, OP_STRREPLACE, OP_STRTORE,
    //OP_RECONCATE, OP_REUNION, OP_REINTER
};

enum FType {
    SMT2, VMT
};

typedef vector<PT*>             PTList;
typedef vector<PTNode*>         PTNodeList;
typedef vector<DG*>             DGList;
typedef vector<DGNode*>         DGNodeList;
typedef pair<string,Type>       Str2Type;
typedef map<string,Type>        Str2TypeMap;
typedef pair<string,size_t>     Str2Uint;
typedef map<string,size_t>      Str2UintMap;
typedef pair<string,DGNode*>    Str2DGNode;
typedef map<string,DGNode*>     Str2DGNodeMap;
typedef pair<string,PTNodeList> Str2PTNodeList;
typedef map<string,PTNodeList>  Str2PTNodeListMap;
typedef queue<PTNode*>          PTNodeQueue;
typedef pair<string,PTNode*>    Str2PTNode;
typedef map<string,PTNode*>     Str2PTNodeMap;
typedef pair<PTNode*,PTNodeList> PTNode2PTNodeList;
typedef map<PTNode*,PTNodeList> PTNode2PTNodeListMap;
typedef pair<PTNode*,bool>      IMP;
typedef vector<IMP>             IMPList;
#endif
