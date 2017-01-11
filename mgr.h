#ifndef _MGR_H_
#define _MGR_H_
#include "typedef.h"
#include "pt.h"
#include "dg.h"

class Mgr{
    public :
        Mgr             (): _pt(new PT("assert")), _indent(3), _gflag(0), _newStrCnt(0), _newReCnt(0) {}
        void            parse(const char*);
        void            buildDG();
        void            writeDG(const string&);
        void            printPT()const{_pt->print(_indent);}
        void            printDG()const;
        string          getNewNodeName(const Type&);
        string          uint2str(size_t cnt);
    private :
        // for parsing
        void            handleConstraint(const vector<string>&);
        PTNode*         handleAssertion(const vector<string>&,size_t,size_t);
        void            handleDeclare(const vector<string>&);
        void            addAssertion(PTNode*);
        PTNode*         buildPTNode(const string&);
        
        // for building dependency graph
        void            buildDG_r(PTNodeQueue&,Str2DGNodeMap&);
        //string          uint2str(size_t cnt);

        PT*             _pt;
        DGList          _dgList;
        Str2VarMap      _typeMap;
        size_t          _indent;
        size_t          _gflag;
        size_t          _newStrCnt;
        size_t          _newReCnt;
};
#endif
