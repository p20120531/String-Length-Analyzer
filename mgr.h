#ifndef _MGR_H_
#define _MGR_H_
#include "typedef.h"
#include "pt.h"
#include "dg.h"

class Mgr{
    public :
        Mgr             (): _pt(new PT("assert",_indent,_gflag)) {_indent=3;_gflag=0;}
        void            parse(const char*);
        void            buildAndWriteDG();
        void            printPT();
        void            printDG();

        PT*&            getPT() {return _pt;}
        const LCList&   getLCList(const size_t& idx) {return _lolcList.at(idx);}
        ofstream&       getLogFile() {return _logFile;}
        void            closeLogFile() {_logFile.close();}
    private :
        // for parsing
        void            handleConstraint(const vector<string>&);
        PTNode*         handleAssertion(const vector<string>&,size_t,size_t);
        void            handleDeclare(const vector<string>&);
        void            addAssertion(PTNode*);
        PTNode*         buildPTNode(const string&);

        PT*             _pt;
        DGList          _dgList;
        LoLCList        _lolcList;
        Str2TypeMap     _typeMap;
        size_t          _indent;
        size_t          _gflag;
        string          _path;
        ofstream        _logFile;
};
#endif
