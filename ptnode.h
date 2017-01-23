#ifndef _PTNODE_H_
#define _PTNODE_H_
#include "typedef.h"

class PTNode {
    friend class PT;
    friend class Mgr;
    public :
        PTNode          (const string& name , const bool& isString) {
            _name = name;
            _isString = isString;
        }
        const string&     getName()const{return _name;}
        const PTNodeList& getChildren()const;
        
        void            setName(const string&);
        void            addChild(PTNode*);
        void            setFlag(const size_t& flag) {_flag=flag;}
        void            lcTraversal(ofstream&,const Str2UintMap&) const;
        virtual DGNode* buildDG() = 0;
        
        virtual void    writeDBG(const size_t&,size_t) const ;
    protected:
        string          _name;
        size_t          _flag;
        bool            _isString;
        PTNodeList      _children;
};

class PTVarIntNode : public PTNode {
    public :
        PTVarIntNode(const string& name) : PTNode(name,0) {}
        void writeDBG(const size_t& , size_t) const ;
        DGNode* buildDG();
};

class PTVarBoolNode : public PTNode {
    public :
        PTVarBoolNode(const string& name) : PTNode(name,0) {}
        void writeDBG(const size_t& , size_t) const ;
        DGNode* buildDG();
};

class PTVarStringNode : public PTNode {
    public :
        PTVarStringNode(const string& name) : PTNode(name,0) {}
        void writeDBG(const size_t& , size_t) const; 
        DGNode* buildDG();
};

class PTConstIntNode : public PTNode {
    public :
        PTConstIntNode(const string& name) : PTNode(name,0) {}
        void writeDBG(const size_t& , size_t) const ;
        DGNode* buildDG();
};

class PTConstBoolNode : public PTNode {
    public :
        PTConstBoolNode(const string& name) : PTNode(name,0) {}
        void writeDBG(const size_t& , size_t) const ;
        DGNode* buildDG();
};

class PTConstStringNode : public PTNode {
    public :
        PTConstStringNode(const string& name) : PTNode(name,0) {}
        void writeDBG(const size_t& , size_t) const;
        DGNode* buildDG();
};

//----------------bool type-------------------

class PTNotNode : public PTNode {
    public :
        PTNotNode(const string& name) : PTNode(name,0) {}
        DGNode* buildDG();
};

class PTEqNode : public PTNode {
    public :
        PTEqNode(const string& name) : PTNode(name,0) {}
        DGNode* buildDG();
};

class PTNotEqNode : public PTNode {
    public :
        PTNotEqNode(const string& name) : PTNode(name,0) {}
        DGNode* buildDG();
};

class PTAndNode : public PTNode {
    public :
        PTAndNode(const string& name) : PTNode(name,0) {}
        DGNode* buildDG();
};

class PTOrNode : public PTNode {
    public :
        PTOrNode(const string& name) : PTNode(name,0) {}
        DGNode* buildDG();
};

class PTIteNode : public PTNode {
    public :
        PTIteNode(const string& name) : PTNode(name,0) {}
        void writeDBG(const size_t& , size_t) const ;
        DGNode* buildDG();
};

class PTLTNode : public PTNode {
    public :
        PTLTNode(const string& name) : PTNode(name,0) {}
        DGNode* buildDG();
};

class PTLTOEQNode : public PTNode {
    public :
        PTLTOEQNode(const string& name) : PTNode(name,0) {}
        DGNode* buildDG();
};

class PTMTNode : public PTNode {
    public :
        PTMTNode(const string& name) : PTNode(name,0) {}
        DGNode* buildDG();
};

class PTMTOEQNode : public PTNode {
    public :
        PTMTOEQNode(const string& name) : PTNode(name,0) {}
        DGNode* buildDG();
};

//-----------------int type-------------------

class PTPlusNode : public PTNode {
    public :
        PTPlusNode(const string& name) : PTNode(name,0) {}
        DGNode* buildDG();
};

class PTMinusNode : public PTNode {
    public :
        PTMinusNode(const string& name) : PTNode(name,0) {}
        DGNode* buildDG();
};

class PTDivNode : public PTNode {
    public :
        PTDivNode(const string& name) : PTNode(name,0) {}
        DGNode* buildDG();
};

//-----------------string type---------------

class PTStrConcateNode : public PTNode {
    public :
        PTStrConcateNode(const string& name) : PTNode(name,1) {}
        DGNode* buildDG();
};

class PTStrLenNode : public PTNode {
    public :
        PTStrLenNode(const string& name) : PTNode(name,1) {}
        DGNode* buildDG();
};

class PTStrInReNode : public PTNode {
    public :
        PTStrInReNode(const string& name) : PTNode(name,1) {}
        DGNode* buildDG();
};

class PTStrNotInReNode : public PTNode {
    public :
        PTStrNotInReNode(const string& name) : PTNode(name,1) {}
        DGNode* buildDG();
};

class PTStrReplaceNode : public PTNode {
    public :
        PTStrReplaceNode(const string& name) : PTNode(name,1) {}
        DGNode* buildDG();
};

class PTStrToReNode : public PTNode {
    public :
        PTStrToReNode(const string& name) : PTNode(name,1) {}
        DGNode* buildDG();
};

//-----------------automaton type-------------------

class PTReConcateNode : public PTNode {
    public :
        PTReConcateNode(const string& name) : PTNode(name,1) {}
        DGNode* buildDG();
};

class PTReUnionNode : public PTNode {
    public :
        PTReUnionNode(const string& name) : PTNode(name,1) {}
        DGNode* buildDG();
};

class PTReInterNode : public PTNode {
    public :
        PTReInterNode(const string& name) : PTNode(name,1) {}
        DGNode* buildDG();
};

#endif
