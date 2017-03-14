#ifndef _AUTOP_MGR_
#define _AUTOP_MGR_
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "utility.h"
using namespace std;

class VmtNode;
class Aut;
class AutOpMgr;

typedef pair<string,VmtNode*> Str2VmtNode;
typedef map<string,VmtNode*>  Str2VmtNodeMap;

class VmtNode{
    friend class Aut;
    friend class AutOpMgr;
    public:
        VmtNode (const string& name): _name(name) {_flag = 0;}
        void addChild(VmtNode*);
        void write(ofstream&,const size_t&);
    private:
        string _name;
        vector<VmtNode*> _children;
        size_t _flag;
};

class Aut{
    friend class AutOpMgr;
    public :
        Aut(){_gflag = 0;}
        Aut(const char* fileName){ parse(fileName); _stateVarCnt = 0; _gflag = 0;}
        Aut(const string& fileName){ parse(fileName.c_str()); _stateVarCnt = 0; _gflag = 0;}
        void parse(const char*);
        void write(const char*);
        void addlen(const size_t&);
        void addepsilon();
        void inter(Aut*,Aut*);
        void unio(Aut*,Aut*);
        void concate(Aut*,Aut*);
        void rename();
    private:
        void vmtTokenize(const string&,vector<string>&);
        VmtNode* buildVmtNode(const string&,size_t,size_t,Str2VmtNodeMap&);
        vector<VmtNode*> _varList;
        vector<VmtNode*> _nxtList;
        vector<VmtNode*> _imdList;
        vector<VmtNode*> _itoList;
        vector<VmtNode*> _defList;
        Str2VmtNodeMap   _vmap;
        size_t _stateVarCnt;
        size_t _gflag;
};

class AutOpMgr{
    public :
        AutOpMgr(){}
        // Converter
        void blif2vmt(const char*,const char*);
        // Automata Operation
        void readCmdFile(const char*);
    private:
        // Converter
        void writeSingleCubeIO(const string&,ofstream&);
        void writeSingleCubeT(const string&,const size_t&,ofstream&);

};

#endif
