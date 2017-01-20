#ifndef _PTNODE_H_
#define _PTNODE_H_
#include "typedef.h"

class PTNode {
    friend class PT;
    friend class Mgr;
    public :
        PTNode          () {}
        PTNode          (const string& name , const bool& isString) : _name(name), _isString(isString) {}
        const string&     getName()const{return _name;}
        const PTNodeList& getChildren()const;
        
        void            setName(const string&);
        void            addChild(PTNode*);
        virtual DGNode* buildDG() = 0;
        
        virtual void    print(const size_t&,size_t) const ;
    protected:
        string          _name;
        size_t          _flag;
        bool            _isString;
        PTNodeList      _children;
};

class PTVarIntNode : public PTNode {
    public :
        PTVarIntNode() {}
        PTVarIntNode(const string& name) : PTNode(name,0) {}
        void print(const size_t& , size_t) const ;
        DGNode* buildDG();
};

class PTVarBoolNode : public PTNode {
    public :
        PTVarBoolNode() {}
        PTVarBoolNode(const string& name) : PTNode(name,0) {}
        void print(const size_t& , size_t) const ;
        DGNode* buildDG();
};

class PTVarStringNode : public PTNode {
    public :
        PTVarStringNode() {}
        PTVarStringNode(const string& name) : PTNode(name,0) {}
        void print(const size_t& , size_t) const; 
        DGNode* buildDG();
};

class PTConstIntNode : public PTNode {
    public :
        PTConstIntNode() {}
        PTConstIntNode(const string& name) : PTNode(name,0) {}
        void print(const size_t& , size_t) const ;
        DGNode* buildDG();
};

class PTConstBoolNode : public PTNode {
    public :
        PTConstBoolNode() {}
        PTConstBoolNode(const string& name) : PTNode(name,0) {}
        void print(const size_t& , size_t) const ;
        DGNode* buildDG();
};

class PTConstStringNode : public PTNode {
    public :
        PTConstStringNode() {}
        PTConstStringNode(const string& name) : PTNode(name,0) {}
        void print(const size_t& , size_t) const;
        DGNode* buildDG();
};

class PTNotNode : public PTNode {
    public :
        PTNotNode() {}
        PTNotNode(const string& name) : PTNode(name,0) {}
        DGNode* buildDG();
};

class PTPlusNode : public PTNode {
    public :
        PTPlusNode() {}
        PTPlusNode(const string& name) : PTNode(name,0) {}
        DGNode* buildDG();
};

class PTNegNode : public PTNode {
    public :
        PTNegNode() {}
        PTNegNode(const string& name) : PTNode(name,0) {}
        DGNode* buildDG();
};

class PTEqNode : public PTNode {
    public :
        PTEqNode() {}
        PTEqNode(const string& name) : PTNode(name,0) {}
        DGNode* buildDG();
};

class PTAndNode : public PTNode {
    public :
        PTAndNode() {}
        PTAndNode(const string& name) : PTNode(name,0) {}
        DGNode* buildDG();
};

class PTOrNode : public PTNode {
    public :
        PTOrNode() {}
        PTOrNode(const string& name) : PTNode(name,0) {}
        DGNode* buildDG();
};

class PTIteNode : public PTNode {
    public :
        PTIteNode() {}
        PTIteNode(const string& name) : PTNode(name,0) {}
        void print(const size_t& , size_t) const ;
        DGNode* buildDG();
};

class PTStrConcateNode : public PTNode {
    public :
        PTStrConcateNode() {}
        PTStrConcateNode(const string& name) : PTNode(name,1) {}
        DGNode* buildDG();
};

class PTStrLenNode : public PTNode {
    public :
        PTStrLenNode() {}
        PTStrLenNode(const string& name) : PTNode(name,1) {}
        DGNode* buildDG();
};

class PTStrInReNode : public PTNode {
    public :
        PTStrInReNode() {}
        PTStrInReNode(const string& name) : PTNode(name,1) {}
        DGNode* buildDG();
};

class PTStrNotInReNode : public PTNode {
    public :
        PTStrNotInReNode() {}
        PTStrNotInReNode(const string& name) : PTNode(name,1) {}
        DGNode* buildDG();
};

class PTStrReplaceNode : public PTNode {
    public :
        PTStrReplaceNode() {}
        PTStrReplaceNode(const string& name) : PTNode(name,1) {}
        DGNode* buildDG();
};

class PTStrToReNode : public PTNode {
    public :
        PTStrToReNode() {}
        PTStrToReNode(const string& name) : PTNode(name,1) {}
        DGNode* buildDG();
};

class PTReConcateNode : public PTNode {
    public :
        PTReConcateNode() {}
        PTReConcateNode(const string& name) : PTNode(name,1) {}
        DGNode* buildDG();
};

class PTReUnionNode : public PTNode {
    public :
        PTReUnionNode() {}
        PTReUnionNode(const string& name) : PTNode(name,1) {}
        DGNode* buildDG();
};

class PTReInterNode : public PTNode {
    public :
        PTReInterNode() {}
        PTReInterNode(const string& name) : PTNode(name,1) {}
        DGNode* buildDG();
};

#endif
