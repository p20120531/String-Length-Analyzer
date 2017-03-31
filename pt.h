#ifndef _PT_H_
#define _PT_H_
#include "ptnode.h"

class PT {
    friend class KaluzaMgr;
    public :
        PT (const string& name,size_t& indent,size_t& gflag): 
            _root(new PTAndNode(name)),_indent(indent),_gflag(gflag) {
            _newDGNodeCnt = 0;
        }
        // Print Function
        void               print() const;
        void               printPTNodeListMap() const;
        // Access Function
        PTNodeQueue&       getPTQ() {return _ptq;}
        IMPQueue&          getIMPQ() {return _impq;}
        Str2DGNodeMap&     getDGMap() {return _dgMap;}
        Str2PTNodeListMap& getPTNodeListMap() {return _ptnodeListMap;}
        string             getNewNodeName();
        // Modify Function
        void               analyzeASCII();
        void               analyze();
        void               addAssertion(PTNode*);
        void               setLevel();
        void               mergeNotEquivalence();
        void               buildPTNodeListMap();
    private :
        PTNode*            _root;
        PTNodeQueue        _ptq;
        IMPQueue           _impq;
        Str2DGNodeMap      _dgMap;
        Str2PTNodeListMap  _ptnodeListMap;
        size_t&            _indent;
        size_t&            _gflag;
        size_t             _newDGNodeCnt;
        
        // for analyze
        bool               _iteDVarLegal;
        bool               _iteCLevel1;
        bool               _strinreRLevel1;
        bool               _strninreRLevel1;
        bool               _streqRLevel1;
        bool               _strneqRLevel1;
        bool               _strlenRLevel2;
        bool               _andCLevel2;
        bool               _ornexist;
        bool               _strinreLCSV;
        bool               _strninreLCSV;
        bool               _streqLCSV;
        bool               _strneqLCSV;
        size_t             _strlenCnt;
        size_t             _strlenEqCnt;
        bool               _streqBothSV;
        bool               _strneqBothSV;
        bool               _strneqOneConst;
};
#endif
