#ifndef _DGMGR_
#define _DGMGR_
#include "typedef.h"

class DGMgr{
    public :
        void parse(const char*);
    private :
        void  handleConstraint(const vector<string>&);
        Node* handleAssertion(const vector<string>&,size_t,size_t);
        void  handleDeclare(const vector<string>&);
        DGList        _dgList;
        Str2VarMap    _typeMap;
};
class DG {
    public :
        Node* successor();
        Node* getSinkNode() {return _sink;}
        void setSinkNode(Node* sink) {_sink = sink;}
    private :
        Node*         _sink;
        Node*         _cur;
};
class Node {
    friend class DG;
    public :
        VarType getTypeByName(const string&);
        void setOpType(const string&);
    private:
        OpType        _optype;
        string        _name;
        string        _regex;
        NodeList      _children;
};
#endif
