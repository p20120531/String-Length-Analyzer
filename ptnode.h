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
        virtual DGNode* buildDG(PTNodeQueue&,Str2DGNodeMap&) = 0;
        
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
        DGNode* buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap );
};

class PTVarBoolNode : public PTNode {
    public :
        PTVarBoolNode() {}
        PTVarBoolNode(const string& name) : PTNode(name,0) {}
        void print(const size_t& , size_t) const ;
        DGNode* buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap );
};

class PTVarStringNode : public PTNode {
    public :
        PTVarStringNode() {}
        PTVarStringNode(const string& name) : PTNode(name,0) {}
        void print(const size_t& , size_t) const; 
        DGNode* buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap );
};

class PTConstIntNode : public PTNode {
    public :
        PTConstIntNode() {}
        PTConstIntNode(const string& name) : PTNode(name,0) {}
        void print(const size_t& , size_t) const ;
        DGNode* buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap );
};

class PTConstBoolNode : public PTNode {
    public :
        PTConstBoolNode() {}
        PTConstBoolNode(const string& name) : PTNode(name,0) {}
        void print(const size_t& , size_t) const ;
        DGNode* buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap );
};

class PTConstStringNode : public PTNode {
    public :
        PTConstStringNode() {}
        PTConstStringNode(const string& name) : PTNode(name,0) {}
        void print(const size_t& , size_t) const;
        DGNode* buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap );
};

class PTNotNode : public PTNode {
    public :
        PTNotNode() {}
        PTNotNode(const string& name) : PTNode(name,0) {}
        DGNode* buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap );
};

class PTPlusNode : public PTNode {
    public :
        PTPlusNode() {}
        PTPlusNode(const string& name) : PTNode(name,0) {}
        DGNode* buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap );
};

class PTNegNode : public PTNode {
    public :
        PTNegNode() {}
        PTNegNode(const string& name) : PTNode(name,0) {}
        DGNode* buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap );
};

class PTEqNode : public PTNode {
    public :
        PTEqNode() {}
        PTEqNode(const string& name) : PTNode(name,0) {}
        DGNode* buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap );
};

class PTAndNode : public PTNode {
    public :
        PTAndNode() {}
        PTAndNode(const string& name) : PTNode(name,0) {}
        DGNode* buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap );
};

class PTOrNode : public PTNode {
    public :
        PTOrNode() {}
        PTOrNode(const string& name) : PTNode(name,0) {}
        DGNode* buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap );
};

class PTIteNode : public PTNode {
    public :
        PTIteNode() {}
        PTIteNode(const string& name) : PTNode(name,0) {}
        void print(const size_t& , size_t) const ;
        DGNode* buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap );
};

class PTStrConcateNode : public PTNode {
    public :
        PTStrConcateNode() {}
        PTStrConcateNode(const string& name) : PTNode(name,1) {}
        DGNode* buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap );
};

class PTStrLenNode : public PTNode {
    public :
        PTStrLenNode() {}
        PTStrLenNode(const string& name) : PTNode(name,1) {}
        DGNode* buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap );
};

class PTStrInReNode : public PTNode {
    public :
        PTStrInReNode() {}
        PTStrInReNode(const string& name) : PTNode(name,1) {}
        DGNode* buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap );
};

class PTStrNotInReNode : public PTNode {
    public :
        PTStrNotInReNode() {}
        PTStrNotInReNode(const string& name) : PTNode(name,1) {}
        DGNode* buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap );
};

class PTStrReplaceNode : public PTNode {
    public :
        PTStrReplaceNode() {}
        PTStrReplaceNode(const string& name) : PTNode(name,1) {}
        DGNode* buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap );
};

class PTStrToReNode : public PTNode {
    public :
        PTStrToReNode() {}
        PTStrToReNode(const string& name) : PTNode(name,1) {}
        DGNode* buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap );
};

class PTReConcateNode : public PTNode {
    public :
        PTReConcateNode() {}
        PTReConcateNode(const string& name) : PTNode(name,1) {}
        DGNode* buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap );
};

class PTReUnionNode : public PTNode {
    public :
        PTReUnionNode() {}
        PTReUnionNode(const string& name) : PTNode(name,1) {}
        DGNode* buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap );
};

class PTReInterNode : public PTNode {
    public :
        PTReInterNode() {}
        PTReInterNode(const string& name) : PTNode(name,1) {}
        DGNode* buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap );
};

#endif
