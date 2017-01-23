#ifndef _PT_H_
#define _PT_H_
#include "ptnode.h"

class PT {
    friend class Mgr;
    public :
        PT (const string& name,size_t& indent,size_t& gflag): 
            _root(new PTAndNode(name)),_indent(indent),_gflag(gflag) {
            _newDGNodeCnt = 0;
        }
        
        PTNode*         getRootNode() const {return _root;}
        PTNodeQueue&    getPTQ() {return _ptq;}
        Str2DGNodeMap&  getDGMap() {return _dgMap;}
        LCList&         getLCList() {return _lcList;}
        PTNodeList&     getLCPTList() {return _lcptList;}
        Str2TypeMap&    getIntVarMap() {return _intVarMap;}
        string          getNewNodeName();
        
        void            writeDBG() const ;
        void            addAssertion(PTNode*);
        void            mergeNotAndStrInRe();
    private :
        PTNode*         _root;
        PTNodeQueue     _ptq;
        Str2DGNodeMap   _dgMap;
        LCList          _lcList;
        PTNodeList      _lcptList;
        Str2TypeMap     _intVarMap;
        size_t&         _indent;
        size_t&         _gflag;
        size_t          _newDGNodeCnt;
};
#endif
