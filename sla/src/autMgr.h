#ifndef _AUT_MGR_
#define _AUT_MGR_
#define INPUT_ENCODE_BIT_NUM 16
#define INIT_STATE_BIT_NUM 4
#define INIT_LVAR_NUM 4
#define EPSILON_ENCODE 0
#define LEFT_ANGLE_BRACKET_ENCODE 1
#define RIGHT_ANGLE_BRACKET_ENCODE 2
#define MAX_SPECIAL_ALPHABET_ENCODE 2
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <assert.h>
#include <bitset>
#include <cstdio>
#include "utility.h"
using namespace std;

class TGEdge;
class TGraph;
class VmtNode;
class Aut;
class AutMgr;

// PARAM   : has _paraList different from _source
// NOPARAM : no need to specify _paraList
enum VmtType {
    INPUT, INPUT_N, STATE, STATE_N, LEN, LEN_N, PARAM, NOPARAM, OTHER
};

enum AType {
    EPSILON, LEFT_ANGLE, RIGHT_ANGLE
};

typedef vector<TGEdge*>       TGEdgeList;
typedef vector<VmtNode*>      VmtNodeList;
typedef set<VmtNode*>         VmtNodeSet;
typedef map<size_t,string>    CubeMap;
typedef vector<VmtNodeSet>    ParaSetList;
typedef vector<VmtNodeList>   VarList;
typedef pair<string,VmtNode*> Str2VmtNode;
typedef map<string,VmtNode*>  Str2VmtNodeMap;

class TGEdge{
    friend class AutMgr;
    friend class TGraph;
    public:
        TGEdge(const size_t& sIdx, const size_t& eIdx): _sIdx(sIdx), _eIdx(eIdx) {}
        void print() const;
        void write(const CubeMap&, const CubeMap&, const vector<string>&, ofstream&);
        void writeExtraBitSpace(const CubeMap&,ofstream&);
        void writeRangeMinterm(const string&, const size_t&, const size_t&, ofstream&);
    private:
        size_t         _sIdx;
        size_t         _eIdx;
        string         _tBitString;
        vector<size_t> _labels;
};

class TGraph{
    friend class AutMgr;
    public:
        TGraph(const char* fileName) {init();parse(fileName);}
        // I/O
        void parse(const char*);
        void write(const char*);
        void write(const string&);
        void print() const;
    private:
        void init();
        bool isAccepting(const string&);
        void parseLabels(const string&, vector<size_t>&);
        size_t label2UTF16(const string&);
        
        set<char>          _numbers;
        map<char,size_t>   _h2dMap;
        CubeMap            _downCubeMap;
        CubeMap            _rangeCubeMap;
        vector<string>     _stateBitStringList;
        size_t             _stateBitNum;
        size_t             _initStateIdx;
        vector<size_t>     _oList;
        TGEdgeList         _tList;
};

class VmtNode{
    friend class Aut;
    friend class AutMgr;
    public:
        VmtNode (const string& name,const VmtType& type=OTHER,const size_t& idx=0): _name(name) 
            {_paraList.assign(6,VmtNodeSet()); _type = type; _idx = idx; _source=0; _flag = 0;}
        VmtNode (const string& name, VmtNode* source) : _name(name), _source(source)
            {_paraList.assign(6,VmtNodeSet()); _type = OTHER; _idx = 0; _flag = 0;}
        void        print(const size_t);
        void        write(ofstream&);
        const       string& getName()            {return _name;}
        void        setType(const VmtType& type) {_type = type;}
        string      getTypeStr();
    private:
        void        merge(VmtNodeSet&, const VmtNodeSet&);
        bool        hasParam();
        bool        haveSameParam(VmtNode*);
        void        addChild(VmtNode*);
        void        clearParam();
        void        buildParam();
        void        writeParam(ofstream&);
        void        shiftStateVar(const size_t&);
        string      _name;
        size_t      _idx;
        VmtType     _type;
        VmtNodeList _children;
        VmtNode*    _source;   // used for _type == PARAM
        ParaSetList _paraList; // input/state/lvar
        size_t      _flag;
};

class Aut{
    friend class VmtNode;
    friend class AutMgr;
    public :
        Aut(){
            cout << "Aut()"<< endl;
            init();
            printVarList();
            //printSpecialAlphabet(EPSILON);
            //printSpecialAlphabet(LEFT_ANGLE);
            //printSpecialAlphabet(RIGHT_ANGLE);
        }
        Aut(const char* fileName){ 
            init();
            parse(fileName);
        }
        Aut(const string& fileName){ 
            init();
            parse(fileName.c_str()); 
        }
        // Static Member Function
        // static member function cannot have const qualifier (don't have this)
        static VmtNode* initSpecialAlphabet(const AType&);
        static VarList  initVarList(const VmtType&);
        static void     printSpecialAlphabet(const AType&);
        static void     printVarList();
        // Non-Static Member Function
        // I/O
        void            test();
        void            print() const;
        void            parse(const char*);
        void            write(const char*);
        void            write(const string&);
        void            writeDefineFun(VmtNode*,ofstream&);
        // Operations
        void            addlen(const string&);
        void            intersect(Aut*,Aut*);
        void            concate(Aut*,Aut*);
        void            replace(Aut*,Aut*,Aut*,Aut*);
        void            mark();
        void            addpred(const string&);
        string          CSNSEquiv(const VmtType&);
    private:
        // Static Member
        static void     expandVarList(const VmtType&);
        static void     vmtTokenize(const string&,vector<string>&,vector<string>&);
        static VmtNode* buildVmtNode(const string&,size_t,size_t,Str2VmtNodeMap&);
        static bool     isReservedString(const string&);
        static void     check(Aut*);
        static void     check(Aut*, const size_t&);
        static void     check(Aut*, Aut*);
        static void     check(Aut*, Aut*, const size_t&);
        
        static size_t   inputBitNum;
        static size_t   stateBitNum;
        static size_t   lvarNum;
        static VarList  input;
        static VarList  state;
        static VarList  lvar;
        static VmtNode* epsilon;
        static VmtNode* leftAngle;
        static VmtNode* rightAngle;
        // Non-Static Member Function
        void            init();
        void            clearParam();
        void            buildParam();
        void            renameDef();
        void            shiftStateVar(const size_t&);
        void            parseDef(const string&, Str2VmtNodeMap&);
        void            parsePred(const string&, size_t&, Str2VmtNodeMap&);
        void            defineFun(const string&, const string&, VmtNodeList&);
        void            defineFun(const string&, const string&, VmtNodeList&, Str2VmtNodeMap&);
        void            defineFun(const string&, const VmtType&, const string&, VmtNodeList&, Str2VmtNodeMap&);
        void            defineFun(const string&, const string&, VmtNodeList&, void (Aut::*)(VmtNode*));
        void            renameITO(const string&, VmtNode*);
        void            renameITOs2Aut(Aut*,Aut*);
        void            integrate(Aut*, Aut*);
        size_t          addStateVar(const size_t&);
        void            addLVar(const size_t&);
        VmtNode*        getI();
        VmtNode*        getO();
        VmtNode*        getT();
        void            setI(VmtNode*);
        void            setO(VmtNode*);
        void            setT(VmtNode*);
        // Non-Static Data Member
        VarList         _state;
        VarList         _lvar;
        VarList         _predBV;
        VarList         _predIV;
        VmtNodeList     _imdList;
        VmtNodeList     _itoList;
        VmtNodeList     _predList;
        Str2VmtNodeMap  _vmap;
        size_t          _stateVarNum;
};

class AutMgr{
    public :
        AutMgr() {_gflag = 0;}
        // Converter
        void dot2blif(const char*,const char*);
        void blif2vmt(const char*,const char*);
        // Automata Operation
        void readCmdFile(const char*);
        size_t& getGFlag() {return _gflag;}
    private:
        // Converter
        void parseCubeList(Aut*, const string&, const vector<string>&, const VmtNodeList&, int&);
        // Data Member
        size_t        _gflag;
};

#endif
