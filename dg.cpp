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
        return "\\~(" + _children[0]->getRegex() + ")";
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

void DGNode::writeCmdFile(ofstream& cmdFile,ofstream& autFile) const
{
    for (DGNodeList::const_iterator it=_children.begin(); it!=_children.end(); ++it)
        if ((*it)->_type != CONST_STRING && (*it)->_type != VAR_STRING && (*it)->_type != AUT_COMPLE) {
            (*it)->writeCmdFile(cmdFile,autFile);
        }
    
    if     (_type == AUT_CONCATE) cmdFile << "concate";  
    else if(_type == AUT_REPLACE) cmdFile << "replace";
    else if(_type == AUT_UNION)   cmdFile << "union";
    else if(_type == AUT_INTER)   cmdFile << "intersect";
    for (DGNodeList::const_iterator it=_children.begin(); it!=_children.end(); ++it) {
        cmdFile << " " << (*it)->_name << ".vmt";
        if ((*it)->_type == CONST_STRING || (*it)->_type == VAR_STRING || (*it)->_type == AUT_COMPLE) {
            autFile << (*it)->_name << " " << (*it)->getRegex() << endl;
        }
    }
    cmdFile << "\nwrite " << _name << endl;
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
                _children.erase(it);
                --it;
            }
            else {
                ++cnt;
            }
        }
        if (cnt == 1) {
            DGNode* n = _children[0];
            if (n->_type == CONST_STRING) {
                assert((n->_children.size()==0));
                _type = CONST_STRING; 
                _regex = n->_regex;
                _children.clear();
            }
            else {
                _type = n->_type;
                _children.clear();
                _children = n->_children;
            }
        }
        else if (cnt == 0) {
            _type = VAR_STRING;
            _children.clear();
        }
    }
    if (_flag != gflag) _flag = gflag;
    else                cout << "this DG is a DAG : at node " << _name << endl;
}
