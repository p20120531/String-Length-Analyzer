#include "dg.h"
string DGNode::getRegex()
{
    if (_type == VAR_STRING)
        return "SIGMASTAR";
    else if(_type == CONST_STRING) {
        assert((_regex.at(0)=='\"' && _regex.at(_regex.size()-1)=='\"'));
        return _regex.substr(1,_regex.size()-2);
    }
    else if(_type == AUT_COMPLE) {
        assert((_children.size() == 1));
        return "?!" + _children[0]->getRegex();
    }
    else if(_type == AUT_CONCATE) {
        string s;
        for (DGNodeList::iterator it=_children.begin();it!=_children.end();++it)
            s += (*it)->getRegex();
        return s;
    }
    else
        cout << "inexist operator appears !!" << endl;
}

DGNode* DGNode::findLeader()
{
    // path compression
    if (_leader != this) 
        _leader = _leader->findLeader();
    return _leader;
}
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
void DGNode::printType() const
{
    switch (_type) {
        case VAR_INT : 
            cout << "VAR_INT";
            break;
        case VAR_BOOL:
            cout << "VAR_BOOL";
            break;
        case VAR_STRING:
            cout << "VAR_STRING";
            break;
        case CONST_INT:
            cout << "CONST_INT";
            break;
        case CONST_BOOL:
            cout << "CONST_BOOL";
            break;
        case CONST_STRING:
            cout << "CONST_STRING";
            break;
        case BOOL_NOT :
            cout << "BOOL_INT";
            break;
        case BOOL_EQ :
            cout << "BOOL_EQ";
            break;
        case BOOL_AND:
            cout << "BOOL_AND";
            break;
        case BOOL_OR:
            cout << "BOOL_OR";
            break;
        case BOOL_ITE:
            cout << "BOOL_ITE";
            break;
        case INT_PLUS:
            cout << "INT_PLUS";
            break;
        case INT_NEG:
            cout << "INT_NEG";
            break;
        case AUT_LEAF:
            cout << "AUT_LEAF";
            break;
        case AUT_CONCATE:
            cout << "AUT_CONCATE";
            break;
        case AUT_INTER:
            cout << "AUT_INTER";
            break;
        case AUT_REPLACE:
            cout << "AUT_REPLACE";
            break;
        case AUT_COMPLE:
            cout << "AUT_COMPLE";
            break;
        default:
            break;
    }
}
void DGNode::printDBG(const size_t& indent,size_t level) const
{
    cout << string(indent*level,' ') 
         << _name << " ";
    printType();
    if (_type == CONST_STRING) cout << " regex=" << _regex;
    cout << endl;
    for (DGNodeList::const_iterator it=_children.begin(); it!=_children.end(); ++it)
        (*it)->printDBG(indent,level+1);
}
void DG::printDBG() const
{
    _sink->printDBG(_indent,0);
}
void DGNode::print() const
{
    string s;
    for (DGNodeList::const_iterator it=_children.begin(); it!=_children.end(); ++it)
        if ((*it)->_type != CONST_STRING && (*it)->_type != VAR_STRING && (*it)->_type != AUT_COMPLE) {
            (*it)->print();
        }
    for (DGNodeList::const_iterator it=_children.begin(); it!=_children.end(); ++it) {
        if ((*it)->_type == CONST_STRING || (*it)->_type == VAR_STRING || (*it)->_type == AUT_COMPLE) {
            if ((*it)->_length)
                s += "addlen " + (*it)->_name + ".blif = " + (*it)->getRegex() 
                   + "\nwrite "+ (*it)->_name + "_length.blif\n";
        }
    }
    
    if     (_type == AUT_CONCATE) s += "concate";  
    else if(_type == AUT_REPLACE) s += "replace";
    else if(_type == AUT_UNION)   s += "union";
    else if(_type == AUT_INTER)   s += "intersect";
    for (DGNodeList::const_iterator it=_children.begin(); it!=_children.end(); ++it) {
        s += " ";
        if ((*it)->_length)
            s += (*it)->_name + "_length.blif";
        else {
            if ((*it)->_type == CONST_STRING || (*it)->_type == VAR_STRING || (*it)->_type == AUT_COMPLE) {
                s += (*it)->_name + ".blif = " + (*it)->getRegex();
            }
            else {
                s += (*it)->_name + ".blif";
            }
        }
    }
    if (_length)
        s += "\naddlen\nwrite " + _name + "_legnth.blif\n";
    else
        s += "\nwrite " + _name + ".blif\n";
    cout << s;
}

void DG::print() const
{
    _sink->print();
}
//-----------------merge-------------
void DGNode::merge(const size_t& gflag)
{
    for (DGNodeList::iterator it=_children.begin(); it!=_children.end(); ++it)
        if ((*it)->_type != CONST_STRING && (*it)->_type != VAR_STRING)
            (*it)->merge(gflag);
    if (_type == AUT_INTER) {
        size_t cnt = 0;
        for (DGNodeList::iterator it=_children.begin(); it!=_children.end(); ++it) {
            if ((*it)->_type == VAR_STRING) {
                assert(((*it)->_children.empty()));
                //FIXME
                delete *it;
                _children.erase(it);
                --it;
            }
            else {
                ++cnt;
            }
        }
        if (cnt == 1) {
            DGNode* n = _children[0];
            _children.clear();
            _type = n->_type;
            _regex= n->_regex;
            //FIXME
            _length |= n->_length;
            _children = n->_children;
        }
    }
    if (_flag != gflag) _flag = gflag;
    else                cout << "this DG is a DAG : at node " << _name << endl;
}
void DG::merge()
{
    _sink->merge(++_flag);
}
