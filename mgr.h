#ifndef _MGR_H_
#define _MGR_H_
#include "typedef.h"
#include "pt.h"
#include "dg.h"

class Mgr{
    public :
        Mgr             (): _pt(new PT("assert")), _indent(3) {}
        void            parse(const char*);
        void            print();
    private :
        void            handleConstraint(const vector<string>&);
        PTNode*         handleAssertion(const vector<string>&,size_t,size_t);
        void            handleDeclare(const vector<string>&);
        void            addAssertion(PTNode*);
        PT*             _pt;
        Str2VarMap      _typeMap;
        size_t          _indent;
};
#endif
