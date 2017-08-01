#ifndef _SMTNODE_H_
#define _SMRNODE_H_
#include "typedef.h"
class SmtNode {
    friend class SmtMgr;
    public :
        SmtNode (const string& name, const SmtType& type): _name(name), _type(type) {
            _flag  = 0;
            _bflag = 0;
        }
        
        // Print Function
        void               print(const size_t&,size_t) const ;
        // Access Function
        const string&      getName()const{return _name;}
        const SmtType&     getType()const{return _type;}
        const size_t&      getFlag()const{return _flag;}
        const size_t&      getBFlag()const{return _bflag;}
        const SmtNodeList& getChildren()const;
        // Modify Function 
        void addChild(SmtNode*);
        void setName(const string& name) {_name = name;}
        void setFlag(const size_t& flag) {_flag=flag;}
        // Conditional Fcuntion
        bool isReturnTypeStr();
        bool isVarStr();
        bool isConstStr();
        bool isStrComparison();

        void writeCVC4PredVar();
        void writeCVC4PredRoot(string&);
        void analyzeASCII() const;
        virtual DTNode* buildDT();
        virtual void buildSmtNodeListMap(Str2SmtNodeListMap&, SmtNode*) const;
    protected:
        string          _name;
        SmtType         _type;
        size_t          _flag;
        size_t          _bflag; // Branch Flag
        SmtNodeList     _children;
};

class SmtConstBoolNode : public SmtNode {
    public :
        SmtConstBoolNode(const string& name) : SmtNode(name,CONST_BOOL) {}
};

class SmtConstIntNode  : public SmtNode {
    public :
        SmtConstIntNode(const string& name) : SmtNode(name,CONST_INT) {}
};

class SmtConstStrNode  : public SmtNode {
    public :
        SmtConstStrNode(const string& name) : SmtNode(name,CONST_STR) {}
        DTNode* buildDT();
};

class SmtVarBoolNode   : public SmtNode {
    public :
        SmtVarBoolNode(const string& name) : SmtNode(name,VAR_BOOL) {}
        void buildSmtNodeListMap(SmtNode*);
};

class SmtVarIntNode : public SmtNode {
    public :
        SmtVarIntNode(const string& name) : SmtNode(name,VAR_INT) {}
        void buildSmtNodeListMap(SmtNode*);
};

class SmtVarStrNode : public SmtNode {
    public :
        SmtVarStrNode(const string& name) : SmtNode(name,VAR_STR) {}
        DTNode* buildDT();
};

class SmtEqNode : public SmtNode {
    public :
        SmtEqNode(const string& name) : SmtNode("=",BOOL_EQ) {}
        DTNode* buildDT();
        void buildSmtNodeListMap(SmtNode*);
};

class SmtNotEqNode : public SmtNode {
    public :
        SmtNotEqNode(const string& name) : SmtNode("!=",BOOL_NEQ) {}
        DTNode* buildDT();
        void buildSmtNodeListMap(SmtNode*);
};

class SmtBoolNode : public SmtNode {
    public :
        SmtBoolNode(const string& name, const SmtType& type) : SmtNode(name,type) {}
        virtual DTNode* buildDT();
};

class SmtIntNode : public SmtNode {
    public :
        SmtIntNode(const string& name, const SmtType& type) : SmtNode(name,type) {}
        virtual DTNode* buildDT();
};

class SmtStrNode : public SmtNode {
    public :
        SmtStrNode(const string& name, const SmtType& type, const DTType& dtype) : SmtNode(name,type),_dtype(dtype) {}
        virtual DTNode* buildDT();
    private:
        DTType _dtype;
};

/////////////////////////// Bool Type ////////////////////////////////

class SmtNotNode : public SmtBoolNode {
    public :
        SmtNotNode(const string& name) : SmtNode("not",BOOL_NOT) {}
};

class SmtAndNode : public SmtBoolNode {
    public :
        SmtAndNode(const string& name) : SmtNode("and",BOOL_AND) {}
        DTNode* buildDT();
};

class SmtOrNode : public SmtBoolNode {
    public :
        SmtOrNode(const string& name) : SmtNode("or",BOOL_OR) {}
        DTNode* buildDT();
        size_t getBeginBranchIdx() {return 0;}
        size_t getEndBranchIdx()   {return _children.size();}
        void   setBranchIdx(const size_t& idx) { _branch = idx;}
    private:
        size_t _branch;
};

class SmtIteNode : public SmtBoolNode {
    public :
        SmtIteNode(const string& name) : SmtNode("ite",BOOL_ITE) { }
        DTNode* buildDT();
        size_t getBeginBranchIdx() {return 1;}
        size_t getEndBranchIdx()   {return 3;}
        void   setBranchIdx(const size_t& idx) { _branch = idx;}
    private:
        size_t _branch;
};

/////////////////////////// Int Type ////////////////////////////////

class SmtLTNode : public SmtIntNode {
    public :
        SmtLTNode(const string& name) : SmtNode("<",BOOL_LT) {}
        DTNode* buildDT();
};

class SmtLTOEQNode : public SmtIntNode {
    public :
        SmtLTOEQNode(const string& name) : SmtNode("<=",BOOL_LTOEQ) {}
        DTNode* buildDT();
};

class SmtMTNode : public SmtIntNode {
    public :
        SmtMTNode(const string& name) : SmtNode(">",BOOL_MT) {}
        DTNode* buildDT();
};

class SmtMTOEQNode : public SmtIntNode {
    public :
        SmtMTOEQNode(const string& name) : SmtNode(">=",BOOL_MTOEQ) {}
        DTNode* buildDT();
};

class SmtPlusNode : public SmtIntNode {
    public :
        SmtPlusNode(const string& name) : SmtNode("+",INT_PLUS) {}
        DTNode* buildDT();
};

class SmtMinusNode : public SmtIntNode {
    public :
        SmtMinusNode(const string& name) : SmtNode("-",INT_MINUS) {}
        DTNode* buildDT();
};

class SmtNegNode : public SmtIntNode {
    public :
        SmtNegNode(const string& name) : SmtNode("-",INT_MINUS) {}
        DTNode* buildDT();
};

class SmtDivNode : public SmtIntNode {
    public :
        SmtDivNode(const string& name) : SmtNode("div",INT_DIV) {}
        DTNode* buildDT();
};

/////////////////////////// String Type ////////////////////////////////

class SmtStrConcateNode : public SmtStrNode {
    public :
        SmtStrConcateNode(const string& name) : SmtStrNode("str.++",STRING_CONCATE,CONCATE) {}
};

class SmtStrLenNode : public SmtStrNode {
    public :
        SmtStrLenNode(const string& name) : SmtStrNode("str.len",INT_STRLEN,OTHER) {}
        DTNode* buildDT();
};

class SmtStrInReNode : public SmtStrNode {
    public :
        SmtStrInReNode(const string& name) : SmtStrNode("str.in.re",BOOL_STRINRE,OTHER) {}
        DTNode* buildDT();
};

class SmtStrNotInReNode : public SmtStrNode {
    public :
        SmtStrNotInReNode(const string& name) : SmtStrNode("str.nin.re",BOOL_STRNINRE,OTHER) {}
        DTNode* buildDT();
};

class SmtStrReplaceNode : public SmtStrNode {
    public :
        SmtStrReplaceNode(const string& name) : SmtStrNode("str.replace",STRING_REPLACE,REPLACE) {}
};

class SmtStrToReNode : public SmtStrNode {
    public :
        SmtStrToReNode(const string& name) : SmtStrNode("str.to.re",REGEX_STRTORE,OTHER) {}
        DTNode* buildDT();
};

class SmtReConcateNode : public SmtStrNode {
    public :
        SmtReConcateNode(const string& name) : SmtReNode("re.++",REGEX_CONCATE,CONCATE) {}
};

class SmtReUnionNode : public SmtStrNode {
    public :
        SmtReUnionNode(const string& name) : SmtReNode("re.union",REGEX_UNION,UNION) {}
};

class SmtReInterNode : public SmtStrNode {
    public :
        SmtReInterNode(const string& name) : SmtReNode("re.inter",REGEX_INTER,INTER) {}
};
#endif
