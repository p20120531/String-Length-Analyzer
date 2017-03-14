#ifndef _KALUZA_MGR_H_
#define _KALUZA_MGR_H_
#include "typedef.h"
#include "pt.h"
#include "dg.h"

class KaluzaMgr{
    public :
        KaluzaMgr       (): _pt(new PT("assert",_indent,_gflag)) {_indent=3;_gflag=0;}
        void            read(const char*);
        void            buildAndWriteDG();
        void            analyzePT();
        
        // Print Function
        void            printPT();
        void            printDG();
        void            printTypeMap();

        // Access Function
        PT*&            getPT() {return _pt;}
        const Str2PTNodeMap& getPTNodeMap() {return _ptnodeMap;}
        const size_t&   getGFlag() {return _gflag;}
        ofstream&       getLogFile() {return _logFile;}
        void            closeLogFile() {_logFile.close();}
    private :
        // For Read
        void            handleConstraint(const vector<string>&);
        PTNode*         handleAssertion(const vector<string>&,size_t,size_t);
        void            handleDeclare(const vector<string>&);
        void            addAssertion(PTNode*);
        PTNode*         buildPTNode(const string&);

        PT*             _pt;
        DGList          _dgList;
        Str2PTNodeMap   _ptnodeMap;
        size_t          _indent;
        size_t          _gflag;
        string          _file;
        string          _path;
        ofstream        _logFile;
};
#endif
