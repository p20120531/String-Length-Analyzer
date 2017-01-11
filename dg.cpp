#include "dg.h"
//----------------merge----------------
// VAR_STRING merges OP_STR
void DGNode::mergeVSOS(DGNode* n)
{
    _type = n->_type;
    assert(_children.empty());
    for (size_t i = 0, size = n->_children.size(); i < size; ++i)
        _children.push_back(n->_children[i]);
    
}
// VAR_STRING merges CONST_STRING
void DGNode::mergeVSCS(DGNode* n)
{
    _type   = CONST_STRING;
    _regex  = n->_regex;
    _isSink = 0;
}

//-----------------print---------------
void DGNode::print() const
{
    bool isAllChildLeaf = 1;
    for (DGNodeList::const_iterator it=_children.begin(); it!=_children.end(); ++it)
        if ((*it)->_type != CONST_STRING && (*it)->_type != VAR_STRING) {
            (*it)->print();
        }
    string s;
    if (_type == OP_STRCONCATE || _type == OP_RECONCATE)
        s += "concate";
    else if(_type == OP_STRREPLACE) s += "replace";
    else if(_type == OP_REUNION)    s += "union";
    else if(_type == OP_REINTER)    s += "intersect";
    if (_length) s += "_l";
    for (DGNodeList::const_iterator it=_children.begin(); it!=_children.end(); ++it) {
        s += " ";
        if ((*it)->_type == CONST_STRING)    s += (*it)->_regex;
        else if ((*it)->_type == VAR_STRING) {
            s += "SIGMASTAR";
            if ((*it)->_length) s += "_l";
        }
        else                                 s += (*it)->_name;
    }
    cout << s << endl;
    cout << "write " << _name << endl;
}

void DG::print() const
{
    _sink->print();
}
