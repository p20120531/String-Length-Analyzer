#ifndef _SMT_MGR_H_
#define _SMT_MGR_H_
#include "typedef.h"
#include "pt.h"
#include "dg.h"

namespace smt {

class SmtMgr{
    public :
        SmtMgr() {} {
            _indent = 3;
            _gflag  = 0;
            _piList.assign(3,SmtNodeList());
            initEscapeSet();
        }
        void            read(const char*,const char*,const bool isAnalyze = 0);
        void            buildAndWriteDG();
        void            analyzePTASCII();
        void            analyzePT();
        void            initEscapeSet();
        string          escape(string);
        
        // Print Function
        void            printPT();
        void            printDG();
        void            printTypeMap();

        // Access Function
        PT*&                  getPT() {return _pt;}
        DG*&                  getDG() {return _dg;}
        const Str2TypeMap&    getTypeMap() {return _typeMap;}
        const size_t&         getGFlag() {return _gflag;}
        ofstream&             getLogFile() {return _logFile;}
        void                  closeLogFile() {_logFile.close();}
    private :
        // For Read
        void            handleConstraint(const vector<string>&);
        PTNode*         handleAssertion(const vector<string>&,size_t,size_t);
        void            handleDeclare(const vector<string>&);
        void            addAssertion(PTNode*);
        PTNode*         buildPTNode(const string&);

        SmtNode*        _root;
        Str2SmtNodeMap  _smtLeafMap;
        vector<SmtNodeList> _piList;
        DTList          _dtList;
        Str2SmtNodeListMap _smtNodeListMap;
        
        PT*             _pt;
        DG*             _dg;
        set<char>       _escapeSet;
        Str2TypeMap     _typeMap;
        size_t          _indent;
        size_t          _gflag;
        string          _path;
        ofstream        _logFile;
};
};
#endif
