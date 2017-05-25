#ifndef _AUTOP_MGR_
#define _AUTOP_MGR_
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <assert.h>
#include "utility.h"
using namespace std;

class VmtNode;
class Aut;
class AutMgr;

// PARAM   : has _paraList different from _source
// NOPARAM : no need to specify _paraList
enum VmtType {
    INPUT, INPUT_N, STATE, STATE_N, LEN, LEN_N, PARAM, NOPARAM, OTHER
};

typedef vector<VmtNode*>      VmtNodeList;
typedef set<VmtNode*>         VmtNodeSet;
typedef pair<string,VmtNode*> Str2VmtNode;
typedef map<string,VmtNode*>  Str2VmtNodeMap;

class VmtNode{
    friend class Aut;
    friend class AutMgr;
    public:
        VmtNode (const string& name,const VmtType& type=OTHER,const size_t& num=0): _name(name) 
            {_paraList.assign(6,VmtNodeSet()); _type = type; _num = num; _source=0; _flag = 0;}
        void addChild(VmtNode*);
        void write(ofstream&);
        void print(const size_t);
        const string& getName() {return _name;}
    private:
        void clearParam();
        void buildParam();
        void writeParam(ofstream&);
        void merge(VmtNodeSet&,VmtNodeSet&);
        void renameStateVar(const size_t&);
        void setType(const VmtType& type) {_type = type;}
        bool hasParam();
        bool haveSameParam(VmtNode*);
        string _name;
        size_t _num;
        VmtType _type;
        VmtNodeList _children;
        // used for _type==PARAM
        VmtNode* _source;
        // iv/sv/lv = input/state/length variable
        vector<VmtNodeSet> _paraList; 
        size_t _flag;
};

class Aut{
    friend class VmtNode;
    friend class AutMgr;
    public :
        Aut(){
            insertXSListAndEpsilon2vmap();
            _snList.assign(4,VmtNodeList());
        }
        Aut(const char* fileName){ 
            _stateVarCnt = 0;
            insertXSListAndEpsilon2vmap();
            _snList.assign(4,VmtNodeList());
            parse(fileName);
        }
        Aut(const string& fileName){ 
            _stateVarCnt = 0; 
            insertXSListAndEpsilon2vmap();
            _snList.assign(4,VmtNodeList());
            parse(fileName.c_str()); 
        }
        void print() const;
        void parse(const char*);
        void write(const char*);
        void write(const string&);
        void addlen(const string&);
        void intersect(Aut*,Aut*);
        void concate(Aut*,Aut*);
        void replace(Aut*,Aut*,Aut*);
        //void unio(Aut*,Aut*);
    private:
        void vmtTokenize(const string&,vector<string>&,vector<string>&);
        static VmtNode* buildVmtNode(const string&,size_t,size_t,Str2VmtNodeMap&);
        void buildVarList(const string&,const size_t&);
        void clearParam();
        void buildParam();
        void insertXSListAndEpsilon2vmap();
        void renameDef();
        void renameStateVar(const size_t&);
        VmtNode* getI();
        VmtNode* getO();
        VmtNode* getT();
        void setI(VmtNode*);
        void setO(VmtNode*);
        void setT(VmtNode*);
        // Data Member
        // 0:s 1:n 2:s.next 3:n.next
        vector<VmtNodeList> _snList;
        VmtNodeList _nxtList;
        VmtNodeList _imdList;
        VmtNodeList _itoList;
        Str2VmtNodeMap   _vmap;
        size_t _stateVarCnt;
};

class AutMgr{
    public :
        AutMgr(){initXSListAndEpsilon(); _gflag = 0;}
        // Converter
        void blif2vmt(const char*,const char*);
        // Automata Operation
        void readCmdFile(const char*);
        void readDefFile(const string&);
        void readPredFile(const string&);
        void mergeStringAndPred(Aut*);
        void initXSListAndEpsilon();
        void print();
        vector<VmtNodeList> _xsList;
        VmtNode* _epsilon;
        static const size_t _stateBitNum = 30;
        size_t _gflag;
    private:
        Str2VmtNodeMap _vmap;
        VmtNodeList _defBVList;
        VmtNodeList _defIVList;
        VmtNodeList _nxtList;
        VmtNodeList _predList;
};

#endif
