#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_
//#define _NDIR_
#define _NLOG_

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <set>
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

enum SmtType {
    VAR_BOOL=0, VAR_INT, VAR_STR,
    CONST_BOOL, CONST_INT, CONST_STR,
    EQ, NEQ,
    // classified by argument type
    NOT=10, AND, OR, ITE,
    LT,LTOEQ,MT,MTOEQ,PLUS,MINUS,NEG,DIV,
    STRCONCATE,STRLEN,STRINRE,STRNINRE,STRREPLACE,STRTORE,
    RECONCATE,REUNION,REINTER
};

enum DTType {
    CONCATE, UNION, INTER, REPLACE, COMPLE, OTHER
};

typedef vector<SmtNode*>        SmtNodeList;
typedef queue<SmtNode*>         SmtNodeQueue;
typedef pair<string,SmtNode*>   Str2SmtNode;
typedef map<string,SmtNode*>    Str2SmtNodeMap;
typedef vector<DT*>             DTList;
typedef pair<string,DTNode*>    Str2DTNode;
typedef map<string,DTNode*>     Str2DTNodeMap;


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
typedef queue<IMP>              IMPQueue;
typedef pair<PTNode*,PTNode*>   PTNodePair;
typedef vector<PTNodePair>      PTNodePairList;
#endif
