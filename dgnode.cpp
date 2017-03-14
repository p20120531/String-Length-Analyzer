#include "dg.h"
#include "kaluzaMgr.h"

extern KaluzaMgr* kmgr;
static const size_t& gflag = kmgr->getGFlag(); 
static PT* pt = kmgr->getPT();
static ofstream& logFile = kmgr->getLogFile();

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
    for (PTNodeList::iterator it=_lengthVarList.begin(); it!=_lengthVarList.end(); ++it)
        n->_lengthVarList.push_back(*it);
    for (IMPList::iterator it=_impList.begin(); it!=_impList.end(); ++it)
        n->_impList.push_back(*it);
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

void DGNode::print(const size_t& indent,size_t level) const
{
    logFile << string(indent*level,' ') 
            << _name << " "
            << getTypeString();
    if (_type == CONST_STRING) logFile << " regex=" << _regex;
    logFile << endl;
    for (DGNodeList::const_iterator it=_children.begin(); it!=_children.end(); ++it)
        (*it)->print(indent,level+1);
}

void DGNode::writeCVC4LeafNode(string& s)
{
    if (_type === AUT_CONCATE) {
        s += " (re.++ ";
        s += ")";
    }
    else if (_type == CONST_STRING) {
        s += " (str.to.re " + _regex + ")";
    }
}

void DGNode::writeCVC4File(Str2TypeMap& typeMap,vector<string>& cvc4StrList, vector<string>& cvc4PredList, const size_t& bflag )
{
    assert((_flag != gflag));
    _flag = gflag;
    Str2TypeMap::iterator it = typeMap.find(_name);
    assert((it == typeMap.end()));
    typeMap.insert(Str2Type(_name,_type));
    
    if (_type == AUT_CONCATE) 
        cvc4StrList.push_back("(assert (= "+_name+" (str.++ "+_children[0]->_name+" "+_children[1]->_name+")))");
    else if (_type == AUT_COMPLE) {
        string s = "(assert (not (str.in.re " + _name;
        _children[0]->writeCVC4LeafNode(s);
        s += ")))";
        cvc4StrList.push_back(s);
    }    
    else if (_type == CONST_STRING)
        cvc4StrList.push_back("(assert (= "+_name+" "+_regex+"))");
    else if (_type == AUT_INTER)

    for (PTNodeList::iterator it=_lengthVarList.begin(); it!=_lengthVarList.end(); ++it) {
        if ( (*it)->getFlag() != gflag) {
            (*it)->setFlag(gflag);
            cvc4PredList.push_back("(assert (= "+(*it)->getName()+" (str.len "+_name+" )))");
            (*it)->writeCVC4Pred(typeMap,cvc4PredList,bflag);
        }
        else {
            #ifndef _NLOG_
                logFile << "[WARNING02]: same Int Variable assigned to 2 different String Variable" << endl;
            #endif 
            cout << "[WARNING02]: same Int Variable assigned to 2 different String Variable" << endl;
        }
    }
    for (IMPList::iterator it=_impList.begin(); it!=_impList.end(); ++it) {
        if ( (*it)->first->getFlag() != gflag) {
            (*it)->first->setFlag(gflag);
            assert(((*it)->first->getType() == VAR_BOOL));
            assert(((*it)->second));
            cvc4PredList.push_back("(assert "+(*it)->first->getName()+")");
            (*it)->first->writeCVC4Pred(typeMap,cvc4PredList,bflag);
        }
        else {
            #ifndef _NLOG_
                logFile << "[WARNING07]: same Bool Variable implied by 2 different String Variable" << endl;
            #endif 
            cout << "[WARNING07]: same Bool Variable implied by 2 different String Variable" << endl;
        }
    }
    for (DGNodeList::iterator it=_children.begin();it!=_children.end();++it)
        (*it)->writeCVC4File(typeMap,cvc4StrList,cvc4PredList,bflag);
}

void DGNode::writeCmdFile(const Str2UintMap& intVarMap, ofstream& cmdFile,ofstream& autFile) const
{
    for (DGNodeList::const_iterator it=_children.begin(); it!=_children.end(); ++it)
        if ((*it)->_type != CONST_STRING && (*it)->_type != VAR_STRING && (*it)->_type != AUT_COMPLE) {
            (*it)->writeCmdFile(intVarMap,cmdFile,autFile);
        }
    cmdFile << "./parse.exe -";
    if     (_type == AUT_CONCATE) cmdFile << "concate";  
    else if(_type == AUT_REPLACE) cmdFile << "replace";
    else if(_type == AUT_UNION)   cmdFile << "union";
    else if(_type == AUT_INTER)   cmdFile << "intersect";
    for (DGNodeList::const_iterator it=_children.begin(); it!=_children.end(); ++it) {
        cmdFile << " " << (*it)->_name << ".vmt";
        if ((*it)->_type == CONST_STRING || (*it)->_type == VAR_STRING || (*it)->_type == AUT_COMPLE) {
            Str2UintMap::const_iterator jt = intVarMap.find((*it)->_name);
            cout << _name << " " << getTypeString() << endl;
            assert((jt != intVarMap.end()));
            autFile << (*it)->_name << " n" << jt->second << " " << (*it)->getRegex() << endl;
        }
    }
    cmdFile << " " << _name << ".vmt" << endl;
    //cmdFile << "\nwrite " << _name << ".vmt" << endl;
}

void DGNode::lcTraversal(Str2UintMap& intVarMap,size_t& cnt) const
{
    for (DGNodeList::const_iterator it=_children.begin(); it!=_children.end(); ++it)
        if ((*it)->_type != CONST_STRING && (*it)->_type != VAR_STRING && (*it)->_type != AUT_COMPLE) {
            (*it)->lcTraversal(intVarMap,cnt);
        }
    for (DGNodeList::const_iterator it=_children.begin(); it!=_children.end(); ++it) {
        if ((*it)->_type == CONST_STRING || (*it)->_type == VAR_STRING || (*it)->_type == AUT_COMPLE) {
            Str2UintMap::const_iterator jt = intVarMap.find((*it)->_name);
            if (jt == intVarMap.end())
                intVarMap.insert(Str2Uint((*it)->_name,cnt++));
        }
    }
}
/*
void DGNode::writeSmt2File(ofstream& smt2File) const
{
    
    for (DGNodeList::const_iterator it=_children.begin(); it!=_children.end(); ++it) {
        const Type& type = (*it)->_type;
        if (type != CONST_STRING && type != VAR_STRING && type != AUT_COMPLE)
            (*it)->writeSmt2File(smt2File);    
    }
    smt2File << "(assert";
    if (_type == AUT_CONCATE) {
        
    }
    else if (_type == AUT_INTER) {
    }
    else if (_type == AUT_COMPLE) {

    }
    else if (_type == )
    {}
    smt2File << ")\n";
    
}
*/

//-----------------merge-------------
void DGNode::merge()
{
    for (DGNodeList::iterator it=_children.begin(); it!=_children.end(); ++it)
        //if ((*it)->_type != CONST_STRING && (*it)->_type != VAR_STRING)
        (*it)->merge();
    
    if (_flag != gflag) _flag = gflag;
    else {
        #ifndef _NLOG_
        logFile << "[WARNING06]: this DG is NOT a DAG : at node " << _name << endl;
        #endif
        cout    << "[WARNING06]: this DG is NOT a DAG : at node " << _name << endl;
    }
    
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
}

void DGNode::renameLengthVar(size_t& lengthVarCnt)
{
    for (PTNodeList::iterator it=_lengthVarList.begin(); it!=_lengthVarList.end(); ++it) {
        (*it)->setLengthVar(lengthVarCnt);
    }
    ++lengthVarCnt;
    for (DGNodeList::iterator it=_children.begin(); it!=_children.end(); ++it)
        (*it)->renameLengthVar(lengthVarCnt);
}
