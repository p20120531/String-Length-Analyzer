#ifndef _PTNODE_H_
#define _PTNODE_H_
#include "typedef.h"

class PTNode {
    friend class PT;
    friend class KaluzaMgr;
    public :
        PTNode          (const string& name, const Type& type) {
            _name = name;
            _type = type;
            _flag = 0;
            _bflag = 0;
        }
        
        // Print Function
        virtual void    print(const size_t&,size_t) const ;
        // Access Function
        const string&     getName()const{return _name;}
        const size_t&     getLevel()const{return _level;}
        const Type&       getType()const{return _type;}
        const size_t&     getFlag()const{return _flag;}
        const size_t&     getBFlag()const{return _bflag;}
        const PTNodeList& getChildren()const;
        // Modify Function 
        void addChild(PTNode*);
        void setName(const string& name) {_name = name;}
        void setFlag(const size_t& flag) {_flag=flag;}
        void setLevel(size_t);
        // Conditional Fcuntion
        bool isReturnTypeStr();
        bool isVarStr();
        bool isConstStr();
        bool isStrComparison();

        void lcTraversal(ofstream&,const Str2UintMap&) const;
        void writeCVC4PredVar();
        void writeCVC4PredRoot(string&);
        virtual DGNode* buildDG() = 0;
        void analyzeASCII() const;
        void analyze(bool&,bool&,bool&,bool&,bool&,bool&,bool&,bool&,bool&,bool&,bool&,bool&,bool&,size_t&,size_t&,bool&,bool&,bool&,int,int) const;
        virtual void buildPTNodeListMap(PTNode*);
    protected:
        string          _name;
        Type            _type;
        size_t          _flag;
        size_t          _bflag; // Branch Flag
        size_t          _level;
        PTNodeList      _children;
};

class PTVarIntNode : public PTNode {
    public :
        PTVarIntNode(const string& name) : PTNode(name,VAR_INT) {}
        void print(const size_t& , size_t) const ;
        DGNode* buildDG();
        void buildPTNodeListMap(PTNode*);
};

class PTVarBoolNode : public PTNode {
    public :
        PTVarBoolNode(const string& name) : PTNode(name,VAR_BOOL) {}
        void print(const size_t& , size_t) const ;
        DGNode* buildDG();
        void buildPTNodeListMap(PTNode*);
};

class PTVarStringNode : public PTNode {
    public :
        PTVarStringNode(const string& name) : PTNode(name,VAR_STRING) {}
        void print(const size_t& , size_t) const; 
        DGNode* buildDG();
};

class PTConstIntNode : public PTNode {
    public :
        PTConstIntNode(const string& name) : PTNode(name,CONST_INT) {}
        void print(const size_t& , size_t) const ;
        DGNode* buildDG();
};

class PTConstBoolNode : public PTNode {
    public :
        PTConstBoolNode(const string& name) : PTNode(name,CONST_BOOL) {}
        void print(const size_t& , size_t) const ;
        DGNode* buildDG();
};

class PTConstStringNode : public PTNode {
    public :
        PTConstStringNode(const string& name) : PTNode(name,CONST_STRING) {}
        void print(const size_t& , size_t) const;
        DGNode* buildDG();
};

//----------------bool type-------------------

class PTNotNode : public PTNode {
    public :
        PTNotNode(const string& name) : PTNode(name,BOOL_NOT) {}
        DGNode* buildDG();
        void buildPTNodeListMap(PTNode*);
};

class PTEqNode : public PTNode {
    public :
        PTEqNode(const string& name) : PTNode(name,BOOL_EQ) {}
        DGNode* buildDG();
        void buildPTNodeListMap(PTNode*);
};

class PTNotEqNode : public PTNode {
    public :
        PTNotEqNode(const string& name) : PTNode(name,BOOL_NEQ) {}
        DGNode* buildDG();
        void buildPTNodeListMap(PTNode*);
};

class PTAndNode : public PTNode {
    public :
        PTAndNode(const string& name) : PTNode(name,BOOL_AND) {}
        DGNode* buildDG();
        void buildPTNodeListMap(PTNode*);
};

class PTOrNode : public PTNode {
    public :
        PTOrNode(const string& name) : PTNode(name,BOOL_OR) {}
        DGNode* buildDG();
        void buildPTNodeListMap(PTNode*);
};

class PTIteNode : public PTNode {
    public :
        PTIteNode(const string& name) : PTNode(name,BOOL_ITE) {}
        void print(const size_t& , size_t) const ;
        DGNode* buildDG();
};

class PTLTNode : public PTNode {
    public :
        PTLTNode(const string& name) : PTNode(name,BOOL_LT) {}
        DGNode* buildDG();
        void buildPTNodeListMap(PTNode*);
};

class PTLTOEQNode : public PTNode {
    public :
        PTLTOEQNode(const string& name) : PTNode(name,BOOL_LTOEQ) {}
        DGNode* buildDG();
        void buildPTNodeListMap(PTNode*);
};

class PTMTNode : public PTNode {
    public :
        PTMTNode(const string& name) : PTNode(name,BOOL_MT) {}
        DGNode* buildDG();
        void buildPTNodeListMap(PTNode*);
};

class PTMTOEQNode : public PTNode {
    public :
        PTMTOEQNode(const string& name) : PTNode(name,BOOL_MTOEQ) {}
        DGNode* buildDG();
        void buildPTNodeListMap(PTNode*);
};

//-----------------int type-------------------

class PTPlusNode : public PTNode {
    public :
        PTPlusNode(const string& name) : PTNode(name,INT_PLUS) {}
        DGNode* buildDG();
        void buildPTNodeListMap(PTNode*);
};

class PTMinusNode : public PTNode {
    public :
        PTMinusNode(const string& name) : PTNode(name,INT_MINUS) {}
        DGNode* buildDG();
        void buildPTNodeListMap(PTNode*);
};

class PTDivNode : public PTNode {
    public :
        PTDivNode(const string& name) : PTNode(name,INT_DIV) {}
        DGNode* buildDG();
        void buildPTNodeListMap(PTNode*);
};

//-----------------string type---------------

class PTStrConcateNode : public PTNode {
    public :
        PTStrConcateNode(const string& name) : PTNode(name,STRING_CONCATE) {}
        DGNode* buildDG();
};

class PTStrLenNode : public PTNode {
    public :
        PTStrLenNode(const string& name) : PTNode(name,INT_STRLEN) {}
        DGNode* buildDG();
};

class PTStrInReNode : public PTNode {
    public :
        PTStrInReNode(const string& name) : PTNode(name,BOOL_STRINRE) {}
        DGNode* buildDG();
};

class PTStrNotInReNode : public PTNode {
    public :
        PTStrNotInReNode(const string& name) : PTNode(name,BOOL_STRNINRE) {}
        DGNode* buildDG();
};

class PTStrReplaceNode : public PTNode {
    public :
        PTStrReplaceNode(const string& name) : PTNode(name,STRING_REPLACE) {}
        DGNode* buildDG();
};

class PTStrToReNode : public PTNode {
    public :
        PTStrToReNode(const string& name) : PTNode(name,REGEX_STRTORE) {}
        DGNode* buildDG();
};

//-----------------automaton type-------------------

class PTReConcateNode : public PTNode {
    public :
        PTReConcateNode(const string& name) : PTNode(name,REGEX_CONCATE) {}
        DGNode* buildDG();
};

class PTReUnionNode : public PTNode {
    public :
        PTReUnionNode(const string& name) : PTNode(name,REGEX_UNION) {}
        DGNode* buildDG();
};

class PTReInterNode : public PTNode {
    public :
        PTReInterNode(const string& name) : PTNode(name,REGEX_INTER) {}
        DGNode* buildDG();
};

#endif
