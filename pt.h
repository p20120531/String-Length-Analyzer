#ifndef _PT_H_
#define _PT_H_
#include "typedef.h"

class PTNode {
    friend class PT;
    friend class Mgr;
    public :
        PTNode          () {}
        PTNode          (const string&);
        VarType         getTypeByName(const string&);
        void            setOpType(const string&);
        void            setName(const string& name) {_name = name;}
        void            print(const size_t&,size_t);
    private:
        OpType          _optype;
        string          _name;
        PTNodeList      _children;
};
class PT {
    friend class Mgr;
    public :
        PT              () {};
        PT              (const string& name): _root(new PTNode(name)) {}
        PTNode*         successor();
        PTNode*         getRootNode() {return _root;}
        void            setRootNode(PTNode* root) {_root = root;}
        void            print(const size_t&);
    private :
        PTNode*         _root;
        PTNode*         _cur;
};
#endif
