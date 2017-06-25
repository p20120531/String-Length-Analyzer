#ifndef _AUT_MGR_
#define _AUT_MGR_
//#define UTF16_ENCODE
#ifdef  UTF16_ENCODE
    #define INPUT_ENCODE_BIT_NUM    16
    #define MAX_ENCODE              65535
#else
    #define INPUT_ENCODE_BIT_NUM    7
    #define MAX_ENCODE              127
#endif
#define PI_NUM                      8
#define EPSILON_ENCODE              0
#define LEFT_ANGLE_BRACKET_ENCODE   1
#define RIGHT_ANGLE_BRACKET_ENCODE  2
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
#define TGRAPH_NDEBUG
#define VMTNODE_NDEBUG
#define AUT_NDEBUG
#define AUT_PARAM_NDEBUG
#define AUT_OP_NDEBUG
#define AUTMGR_NDEBUG

class TGEdge;
class TGraph;
class VmtNode;
class Aut;
class AutMgr;

// PARAM : has _paramList different from _source
enum VmtType {
    // LEAF  (leaf node)
    // PI    (primary input)
    INPUT , EXIST  , STATE  , LEN   , STATE_N , LEN_N , PREDBV , PREDIV , 
    // CONST (constant)
    NUM   , CONST0 , CONST1 , PARAM , SPECIAL , 
    // IMD   (intermediate node)
    NOT=30, NEG    , AND    , OR    , PLUS    , MINUS , LT     , LTOEQ  , EQ, MTOEQ, MT, EXCM, MODULE
};

enum AType {
    EPSILON, LEFT_ANGLE, RIGHT_ANGLE
};

enum AutOpType {
    ADDLEN, SUBSTR, INTERSECT, CONCATE, REPLACE, REPLACE_A4
};

typedef vector<TGEdge*>       TGEdgeList;
typedef vector<VmtNode*>      VmtNodeList;
typedef set<VmtNode*>         VmtNodeSet;
typedef map<size_t,string>    CubeMap;
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
        void writeRangeMinterm(const size_t&, const size_t&, ofstream&);
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
        size_t label2Decimal(const string&);
        
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
        
        VmtNode (const string& name,const size_t& idx=0): _name(name), _idx(idx) 
        { _paramList.assign(PI_NUM,VmtNodeList()); _type = getType(name) ; _source=0; _flag = 0; }

        VmtNode (const string& name,VmtNode* source) : _name(name), _source(source)
        { _paramList.assign(PI_NUM,VmtNodeList()); _type = PARAM; _idx = 0 ; _flag = 0; }
        
        void         print(const size_t&);
        void         write(const size_t&,ofstream&);
        const        string& getName()            {return _name;}
        void         setType(const VmtType& type) {_type = type;}
        VmtType      getType(const string& name);
    private:
        bool         hasParam();
        bool         haveSameParam(VmtNode*);
        void         addChild(VmtNode*);
        void         clearParam(const size_t&);
        void         buildParam(const size_t&);
        void         collectPARAM(VmtNodeList&);
        void         mergeNEG2MINUS();
        void         writeParamHead(ofstream&);
        void         writeParamBody(const string&,ofstream&);
        void         shiftStateVar(const size_t&);
        //TODO construct sequential circuit
        void         traverse();
        void         writeBLIF(ofstream&);
        string       _name;
        size_t       _idx;
        VmtType      _type;
        VmtNodeList  _children;
        VmtNode*     _source;    // used for _type == PARAM
        VarList      _paramList; // 0-7 follow the order of VmtType
        size_t       _flag;
};

class Aut{
    friend class VmtNode;
    friend class AutMgr;
    public :
        Aut(){
            init();
        }
        Aut(const char* fileName){ 
            init(fileName);
            parse(fileName);
        }
        Aut(const string& fileName){ 
            init(fileName);
            parse(fileName.c_str()); 
        }
        Aut ( const string& fileName, const string& lvarIdxStr, const AutOpType& type ){
            assert( (type == ADDLEN) );
            init(fileName);
            parse(fileName.c_str());
            addlen(lvarIdxStr);
        }
        Aut ( const string& fileName, const string& n0, const string& n1, const AutOpType& type ) {
            assert( (type == SUBSTR) );
            init(fileName);
            parse(fileName.c_str());
            substr(n0,n1);
        }
        Aut ( Aut* a1, Aut* a2, const size_t& alpha, const AutOpType& type) {
            assert( (type == REPLACE) );
            init();
            replace(a1,a2,alpha);
        }
        Aut ( Aut* a1, Aut* a2, const AutOpType& type ){
            init();
            if (type == INTERSECT) {
                intersect(a1,a2);
            }
            else if (type == CONCATE) {
                concate(a1,a2);
            }
            else {
                assert( (type == REPLACE_A4) );
                replace_A4(a1,a2);
            }
        }

        // Static Member Function
        // static member function cannot have const qualifier (don't have this)
        static VmtNode* initConst(const VmtType&);
        static VmtNode* initSpecialAlphabet(const AType&);
        static VarList  initPIList();
        static void     printStaticDataMember();
        static bool     isPI(const VmtType&);
        static bool     isCurPI(const VmtType&);
        static bool     isLEAF(const VmtType&);
        static bool     isIMD(const VmtType&);
        static bool     isPISymbol(const char&);
        static bool     isSpecialString(const string&);
        static string   getPISymbol(const VmtType&);
        static string   getPISymbol(const size_t&);
        static string   getTypeStr(const VmtType&);
        static string   getTypeStr(const size_t&);
        static VmtType  getPITypeByName(const string&);
        // Non-Static Member Function
        // I/O
        void            test();
        void            print() const;
        void            printPARAMList() const;
        void            printPIList() const;
        void            parse(const char*);
        void            write(const char*);
        void            write(const string&);
        void            writeDeclareFun(const VmtType&, ofstream&);
        void            writeNextFun(const VmtType&, int&, ofstream&);
        void            writeDefineFun(VmtNode*,ofstream&,const bool& needParam=1);
        // Operations
        string          CSNSEquiv(const VmtType&);
        // Unary 
        void            addlen(const string&);
        size_t          mark();
        void            prefix(const string&);
        void            suffix(const string&);
        void            substr(const string&, const string&);
        void            addpred(const string&);
        void            isempty(const string&);
        // Binary
        void            intersect(Aut*,Aut*);
        void            concate(Aut*,Aut*);
        void            replace(Aut*,Aut*,const size_t&);
        void            replace_A4(Aut*,Aut*);
    private:
        // Static Member
        static void     expandPIList(const VmtType&,const size_t&);
        static void     vmtTokenize(const string&,vector<string>&,vector<string>&);
        static VmtNode* buildVmtNode(const string&,size_t,size_t,Str2VmtNodeMap&);
        static bool     isReservedString(const string&);
        static void     check(Aut*);
        static void     check(Aut*, const size_t&);
        static void     check(Aut*, Aut*);
        static void     check(Aut*, Aut*, const size_t&);
        
        static size_t   inputBitNum;
        //static size_t   evarNum;
        //static size_t   stateBitNum;
        //static size_t   lvarNum;
        //static size_t   predBVNum;
        //static size_t   predIVNum;
        //static VarList  input;  // shared
        //static VarList  evar;   // repeated
        //static VarList  state;  // shared
        //static VarList  lvar;   // non-repeated
        //static VarList  predBV; // repeated
        //static VarList  predIV; // repeated
        static VarList  piList; // 0-7 follows the order of VmtType
        static VmtNode* const0;
        static VmtNode* const1;
        static VmtNode* epsilon;
        static VmtNode* leftAngle;
        static VmtNode* rightAngle;
        // Non-Static Member Function
        void            init(const string& fileName="NONAME");
        void            initVMap();
        void            buildVMap(const VmtType&);
        void            buildVMap(const VmtNodeList&);
        void            assignGlobalPIList(const VmtType&);
        void            clearParam();
        void            buildParam();
        void            collectPARAM();
        void            mergeNEG2MINUS();
        void            renameDef();
        void            shiftStateVar(const size_t&);
        void            parseDef(const string&, Str2VmtNodeMap&);
        void            parsePred(const string&, size_t&, Str2VmtNodeMap&);
        void            defineFun(const string&, const string&, VmtNodeList&);
        void            defineFun(const string&, const string&, VmtNodeList&, Str2VmtNodeMap&);
        void            defineFun(const string&, const VmtType&, const string&, VmtNodeList&, Str2VmtNodeMap&);
        void            defineFun(const string&, const string&, VmtNodeList&, void (Aut::*)(VmtNode*));
        void            addParamNode(const string&, VmtNode*);
        void            addParamNode(const string&, VmtNode*, const size_t&, const bool&);
        void            renameITO(const string&, VmtNode*);
        void            renameITO1Aut();
        void            renameITOs2Aut(Aut*,Aut*);
        void            integrate(Aut*, Aut*);
        void            integrateMerge(const VmtType&,Aut*,Aut*);
        size_t          addEVar(const size_t&);
        size_t          addStateVar(const size_t&);
        void            addLVar(const size_t&);
        VmtNode*        getI();
        VmtNode*        getO();
        VmtNode*        getT();
        void            setI(VmtNode*);
        void            setO(VmtNode*);
        void            setT(VmtNode*);
        // Non-Static Data Member
        //VarList         _evar;
        //VarList         _state;
        //VarList         _lvar;
        //VarList         _predBV;
        //VarList         _predIV;
        VarList         _piList;
        VmtNodeList     _imdList;
        VmtNodeList     _itoList;
        VmtNodeList     _predList;
        VmtNodeList     _PARAMList;
        Str2VmtNodeMap  _vmap;
        size_t          _stateVarNum;
        //bool            _eUsed;
        //bool            _lUsed;
        //bool            _rUsed;
        string          _name;
};

class AutMgr{
    public :
        AutMgr() {_gflag = 0;}
        // Converter
        void    dot2blif(const char*,const char*);
        void    blif2vmt(const char*,const char*);
        void    aig2vmt (const char*,const char*);
        //void    vmt2blif (const char*,const char*);
        // Automata Operation
        void    readCmdFile(const char*);
        size_t& getGFlag() {return _gflag;}
    private:
        // Converter
        void    parseCubeList(Aut*, const string&, const vector<string>&, const VmtNodeList&, int&);
        string  cube2vmt(const string& cube, const VmtNodeList&);
        // Data Member
        size_t  _gflag;
};
#endif
