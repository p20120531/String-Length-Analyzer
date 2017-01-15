#ifndef _MGR_H_
#define _MGR_H_
#include "typedef.h"
#include "pt.h"
#include "dg.h"

class Mgr{
    public :
        Mgr             (): _pt(new PT("assert")), _indent(3), _dgFlag(10), _newDGNodeCnt(0) {}
        void            parse(const char*);
        void            buildDG();
        void            writeDG(const string&);
        void            printPT()const;
        void            printDG()const;
        string          getNewNodeName();
    private :
        // for parsing
        void            handleConstraint(const vector<string>&);
        PTNode*         handleAssertion(const vector<string>&,size_t,size_t);
        void            handleDeclare(const vector<string>&);
        void            addAssertion(PTNode*);
        PTNode*         buildPTNode(const string&);
        
        // for building dependency graph
        void            buildDG_r(PTNodeQueue&,Str2DGNodeMap&);

        PT*             _pt;
        DGList          _dgList;
        Str2VarMap      _typeMap;
        size_t          _indent;
        size_t          _dgFlag;
        size_t          _newDGNodeCnt;
};
#endif
