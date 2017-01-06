#ifndef _PTMGR_
#define _PTMGR_
#include "typedef.h"

class PTMgr{
    public :
        void    parse(const char*);
        void    print();
    private :
        void    handleConstraint(const vector<string>&);
        PTNode* handleAssertion(const vector<string>&,size_t,size_t);
        void    handleDeclare(const vector<string>&);
        PTList        _ptList;
        Str2VarMap    _typeMap;
};
class PT {
    public :
        PT () {_indent = 3;}
        PTNode* successor();
        PTNode* getRootNode() {return _root;}
        void    setRootNode(PTNode* root) {_root = root;}
        void    print();
    private :
        PTNode*         _root;
        PTNode*         _cur;
        size_t          _indent;
};
class PTNode {
    friend class PT;
    friend class PTMgr;
    public :
        PTNode () {}
        PTNode (const string&);
        VarType getTypeByName(const string&);
        void setOpType(const string&);
        void setName(const string& name) {_name = name;}
        void print(const size_t&,size_t);
    private:
        OpType        _optype;
        string        _name;
        string        _regex;
        PTNodeList    _children;
};
#endif
