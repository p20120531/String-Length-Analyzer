#ifndef _DG_H_
#define _DG_H_
#include "typedef.h"

class DG {
    friend class Mgr;
    public :
        DGNode* successor();
        DGNode* getSinkNode() {return _sink;}
        void setSinkNode(DGNode* sink) {_sink = sink;}
    private :
        DGNode*         _sink;
        DGNode*         _cur;
};
class DGNode {
    friend class DG;
    friend class Mgr;
    public :
        VarType getTypeByName(const string&);
        void setOpType(const string&);
    private:
        OpType        _optype;
        string        _name;
        string        _regex;            // char(0): epsilon , char(1): < , char(2): > , char(3): sigma*
        string        _lengthConstraint; // "" if no need , word if assigned to a variable , integer otherwise
        DGNodeList    _children;
};
#endif
