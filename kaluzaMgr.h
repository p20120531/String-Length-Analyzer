#ifndef _KALUZA_MGR_H_
#define _KALUZA_MGR_H_
#include "typedef.h"
#include "pt.h"
#include "dg.h"

class KaluzaMgr{
    public :
        KaluzaMgr       (): _pt(new PT("assert",_indent,_gflag)) {_indent=3;_gflag=0;}
        void            read(const char*,const char*);
        void            buildAndWriteDG();
        void            analyzePTASCII();
        void            analyzePT();
        
        // Print Function
        void            printPT();
        void            printDG();
        void            printTypeMap();

        // Access Function
        PT*&            getPT() {return _pt;}
        DG*&            getDG() {return _dg;}
        const Str2TypeMap& getTypeMap() {return _typeMap;}
        const size_t&   getGFlag() {return _gflag;}
        const vector<string>& getBVList() {return _bvList;}
        const vector<string>& getIVList() {return _ivList;}
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
        DG*             _dg;
        Str2TypeMap     _typeMap;
        vector<string>  _bvList;
        vector<string>  _ivList;
        vector<string>  _svList;
        size_t          _indent;
        size_t          _gflag;
        string          _file;
        string          _path;
        ofstream        _logFile;
};
#endif
