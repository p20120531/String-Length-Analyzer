#include "dg.h"
#include "mgr.h"

extern Mgr* mgr;
static ofstream& logFile = mgr->getLogFile();

string DGNode::getRegex()
{
    if (_type == VAR_STRING)
        return ".*";
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

void DGNode::setLeader(DGNode* n)
{
    _leader = n;
    if (_length || n->_length) {
        _length = 1;
        n->_length = 1;
    }
}

DGNode* DGNode::findLeader()
{
    // path compression
    if (_leader != this) 
        _leader = _leader->findLeader();
    return _leader;
}

//-----------------print---------------
const char* DGNode::getTypeString() const
{
    switch (_type) {
        case VAR_INT : 
            return  "VAR_INT";
        case VAR_BOOL:
            return  "VAR_BOOL";
        case VAR_STRING:
            return  "VAR_STRING";
        case CONST_INT:
            return  "CONST_INT";
        case CONST_BOOL:
            return  "CONST_BOOL";
        case CONST_STRING:
            return  "CONST_STRING";
        case BOOL_NOT :
            return  "BOOL_INT";
        case BOOL_EQ :
            return  "BOOL_EQ";
        case BOOL_AND:
            return  "BOOL_AND";
        case BOOL_OR:
            return  "BOOL_OR";
        case BOOL_ITE:
            return  "BOOL_ITE";
        case INT_PLUS:
            return  "INT_PLUS";
        case INT_NEG:
            return  "INT_NEG";
        case AUT_CONCATE:
            return  "AUT_CONCATE";
        case AUT_INTER:
            return  "AUT_INTER";
        case AUT_REPLACE:
            return  "AUT_REPLACE";
        case AUT_COMPLE:
            return  "AUT_COMPLE";
        default:
            return  "invalid";
    }
}

void DGNode::writeDBG(const size_t& indent,size_t level) const
{
    logFile << string(indent*level,' ') 
            << _name << " "
            << getTypeString();
    if (_type == CONST_STRING) logFile << " regex=" << _regex;
    logFile << endl;
    for (DGNodeList::const_iterator it=_children.begin(); it!=_children.end(); ++it)
        (*it)->writeDBG(indent,level+1);
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
    cmdFile << "\nwrite " << _name << ".vmt" << endl;
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
    else                logFile << "this DG is a DAG : at node " << _name << endl;
}
