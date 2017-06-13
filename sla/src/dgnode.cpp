#include "dg.h"
#include "kaluzaMgr.h"

extern KaluzaMgr*    kmgr;
static const size_t& gflag   = kmgr->getGFlag(); 
static PT*&          pt      = kmgr->getPT();
static DG*&          dg      = kmgr->getDG();
static ofstream&     logFile = kmgr->getLogFile();

string DGNode::getRegex() const
{
    if (_type == VAR_STRING)
        return ".*";
    else if(_type == CONST_STRING) {
        assert((_regex.at(0)=='\"' && _regex.at(_regex.size()-1)=='\"'));
        string s = _regex.substr(1,_regex.size()-2);
        return s;
        /*
        if (s == "") return "EPSILON";
        else {
            string ret = "";
            for (size_t i=0,size=s.size(); i<size; ++i) {
                if (escapeSet.find(s[i]) != escapeSet.end())
                    ret += "\\" + s[i];
                else 
                    ret += s[i];
            }
            return ret;
        }
        */
    }
    else if(_type == AUT_COMPLE) {
        assert((_children.size() == 1));
        return "~(" + _children[0]->findLeader()->getRegex() + ")";
    }
    else if(_type == AUT_CONCATE) {
        string s;
        for (DGNodeList::const_iterator it=_children.begin();it!=_children.end();++it)
            s += (*it)->findLeader()->getRegex();
        return s;
    }
    else
        cout << "inexist operator appears !!" << endl;
}

void DGNode::setLeader(DGNode* n)
{
    _leader = n;
    for (PTNodePairList::iterator it=_lengthVarList.begin(); it!=_lengthVarList.end(); ++it)
        n->_lengthVarList.push_back(*it);
    for (PTNodePairList::iterator it=_assertionList.begin(); it!=_assertionList.end(); ++it)
        n->_assertionList.push_back(*it);
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
    #ifndef _NLOG_
    logFile << string(indent*level,' ') 
            << _name << " "
            << getTypeString();
    #endif
    if (_type == CONST_STRING) {
        #ifndef _NLOG_
        logFile << " regex=" << _regex;
        #endif
    }
    #ifndef _NLOG_
    logFile << endl;
    #endif
    for (DGNodeList::const_iterator it=_children.begin(); it!=_children.end(); ++it)
        (*it)->findLeader()->print(indent,level+1);
}

void DGNode::printLengthVarList() const
{
    #ifndef _NLOG_
        logFile << "DGNode: name=" << _name << endl;
    #endif
    for (PTNodePairList::const_iterator it=_lengthVarList.begin(); it!=_lengthVarList.end(); ++it)
        (*it).second->print(3,0);
    for (DGNodeList::const_iterator it=_children.begin(); it!=_children.end(); ++it)
        (*it)->findLeader()->printLengthVarList();
}

void DGNode::printAssertionList() const
{
    #ifndef _NLOG_
        logFile << "DGNode: name=" << _name << endl;
    #endif
    for (PTNodePairList::const_iterator it=_assertionList.begin(); it!=_assertionList.end(); ++it)
        (*it).second->print(3,0);
    for (DGNodeList::const_iterator it=_children.begin(); it!=_children.end(); ++it)
        (*it)->findLeader()->printAssertionList();
}

void DGNode::writeCVC4LeafNode(string& s)
{
    if (_type == AUT_CONCATE) {
        s += " (re.++";
        for (DGNodeList::iterator it=_children.begin(); it!=_children.end(); ++it)
            (*it)->findLeader()->writeCVC4LeafNode(s);
        s += ")";
    }
    else if (_type == CONST_STRING) {
        assert((_children.size() == 0));
        s += " (str.to.re " + _regex + ")";
    }
    else {
        #ifndef _NLOG_
            logFile << "[WARNING:DGNode::writeCVC4LeafNode] invalid type=" << _type << " at LeafDGNode=" << _name << endl;
        #endif
        cout << "[WARNING:DGNode::writeCVC4LeafNode] invalid type=" << _type << " at LeafDGNode=" << _name << endl;
    }
}

void DGNode::writeCVC4File()
{
    assert((_flag != gflag));
    _flag = gflag;
    Str2TypeMap& typeMap = dg->getTypeMap();
    vector<string>& cvc4StrList = dg->getCVC4StrList();
    vector<string>& cvc4PredList = dg->getCVC4PredList();
    set<string>& bvStrSet = dg->getBVStrSet(); 
    set<string>& ivStrSet = dg->getIVStrSet(); 

    Str2TypeMap::iterator it = typeMap.find(_name);
    assert((it == typeMap.end()));
    assert((_type == AUT_CONCATE || _type == AUT_INTER || _type == AUT_COMPLE || _type == CONST_STRING || _type == VAR_STRING));
    typeMap.insert(Str2Type(_name,VAR_STRING));
    
    if (_type == AUT_CONCATE) 
        cvc4StrList.push_back("(assert (= "+_name+" (str.++ "+_children[0]->findLeader()->_name+" "+_children[1]->findLeader()->_name+")))");
    else if (_type == AUT_INTER) {
        assert((_children.size() == 2));
        cvc4StrList.push_back("(assert (= "+_name+" "+_children[0]->findLeader()->_name+"))");
        cvc4StrList.push_back("(assert (= "+_children[0]->findLeader()->_name+" "+_children[1]->findLeader()->_name+"))");
    }
    else if (_type == AUT_COMPLE) {
        assert((_children.size() == 1));
        string s = "(assert (not (str.in.re " + _name;
        _children[0]->findLeader()->writeCVC4LeafNode(s);
        s += ")))";
        cvc4StrList.push_back(s);
    }    
    else if (_type == CONST_STRING)
        cvc4StrList.push_back("(assert (= "+_name+" "+_regex+"))");
    else {
        assert((_type == VAR_STRING));
        /*
        #ifndef _NLOG_
            logFile << "[WARNING08]: invalid type=" << _type << " at DGNode=" << _name << endl;
        #endif
        cout << "[WARNING08]: invalid type=" << _type << " at DGNode=" << _name << endl;
        */
    }

    for (PTNodePairList::iterator it=_lengthVarList.begin(); it!=_lengthVarList.end(); ++it) {
        const Type& type = (*it).second->getType();
        if (type == CONST_INT) {
            cvc4PredList.push_back("(assert (= "+(*it).second->getName()+" (str.len "+_name+"))) ; len "+itos(_lengthVarCnt));
            //cvc4PredList.push_back("(assert (= "+(*it).second->getName()+" (str.len "+_name+"))) ; cstrlen "+itos(_lengthVarCnt));
        }
        else {
            assert((type == VAR_INT));
            set<string>::iterator jt = ivStrSet.find((*it).second->getName());
            if (jt == ivStrSet.end()) {
                ivStrSet.insert((*it).second->getName());
                string s = "(assert";
                (*it).first->writeCVC4PredRoot(s);
                s += ") ; len "+itos(_lengthVarCnt);
                //s += ") ; vstrlen "+itos(_lengthVarCnt);
                cvc4PredList.push_back(s);
            }
            else {
                #ifndef _NLOG_
                    logFile << "[WARNING:DGNode::writeCVC4File] same Int Variable assigned to 2 different String Variable" << endl;
                #endif 
                cout << "[WARNING:DGNode::writeCVC4File] same Int Variable assigned to 2 different String Variable" << endl;
            }
        }
    }
    for (PTNodePairList::iterator it=_assertionList.begin(); it!=_assertionList.end(); ++it) {
        assert(((*it).second->getType() == VAR_BOOL));
        set<string>::iterator jt = bvStrSet.find((*it).second->getName());
        if (jt == bvStrSet.end()) {
            bvStrSet.insert((*it).second->getName());
            // FIXME
            //string s = "(assert";
            //(*it).first->writeCVC4PredRoot(s);
            //s += ")";
            string s = "(assert " + (*it).second->getName() + ")";
            cvc4PredList.push_back(s);
            (*it).second->setFlag(gflag);
            (*it).second->writeCVC4PredVar();
            /*
            string s = "(assert";
            (*it).first->writeCVC4PredRoot(s);
            s += ")";
            cvc4PredList.push_back(s);
            */
        }
        else {
            #ifndef _NLOG_
                logFile << "[WARNING:DGNode::writeCVC4File] same Bool Variable implied by 2 different String Variable" << endl;
            #endif 
            cout << "[WARNING:DGNode::writeCVC4File] same Bool Variable implied by 2 different String Variable" << endl;
        }
    }
    if (_type == AUT_COMPLE) return;
    for (DGNodeList::iterator it=_children.begin();it!=_children.end();++it)
        (*it)->findLeader()->writeCVC4File();
}

void DGNode::writeCmdFile(ofstream& cmdFile,ofstream& autFile)
{
    assert((_flag != gflag)); // Not DAG
    _flag = gflag;
    if (_type == VAR_STRING || _type == CONST_STRING || _type == AUT_COMPLE) {
        if (this == dg->getSinkNode()) {
            if (_lengthVarCnt != -1) {
                cmdFile << "addlen " << _name << " " << _lengthVarCnt << endl;
                // intermediate file for debug
                cmdFile << "write " << _name << "_l" << endl;
            }
            else {
                cmdFile << "read " << _name << endl;
                // intermediate file for debug
                cmdFile << "write " << _name << endl;
            }
        }
        else {
            if (_lengthVarCnt != -1) {
                cmdFile << "addlen " << _name << " " << _lengthVarCnt << endl;
                cmdFile << "write " << _name << "_l" << endl;
            }
        }
        autFile << _name << " " << kmgr->escape(getRegex()) << endl;
        return;
    }
    for (DGNodeList::iterator it=_children.begin(); it!=_children.end(); ++it) {
        (*it)->findLeader()->writeCmdFile(cmdFile,autFile);
    }
    if     (_type == AUT_CONCATE) cmdFile << "concate";  
    else if(_type == AUT_REPLACE) cmdFile << "replace";
    else if(_type == AUT_UNION)   cmdFile << "union";
    else if(_type == AUT_INTER)   cmdFile << "intersect";
    for (DGNodeList::iterator it=_children.begin(); it!=_children.end(); ++it) {
        cmdFile << " " << (*it)->findLeader()->getName();
        if ((*it)->findLeader()->_lengthVarCnt != -1) cmdFile << "_l";
    }
    cmdFile << endl << "write " << _name << endl;
    if (_lengthVarCnt != -1) {
        cmdFile << "addlen " << _name << " " << _lengthVarCnt << endl;
        cmdFile << "write " << _name << "_l" << endl;
    }
}

//-----------------merge-------------
void DGNode::merge()
{
    for (DGNodeList::iterator it=_children.begin(); it!=_children.end(); ++it)
        //if ((*it)->_type != CONST_STRING && (*it)->_type != VAR_STRING)
        (*it)->findLeader()->merge();
    
    if (_flag != gflag) _flag = gflag;
    else {
        #ifndef _NLOG_
        logFile << "[WARNING:DGNode::merge] this DG is NOT a Tree : at node " << _name << endl;
        #endif
        cout    << "[WARNING:DGNode::merge] this DG is NOT a Tree : at node " << _name << endl;
    }
    
    if (_type == AUT_INTER) {
        size_t cnt = 0;
        for (DGNodeList::iterator it=_children.begin(); it!=_children.end(); ++it) {
            if ((*it)->findLeader()->_type == VAR_STRING) {
                assert(((*it)->findLeader()->_children.empty()));
                //FIXME
                _children.erase(it);
                --it;
            }
            else {
                ++cnt;
            }
        }
        if (cnt == 1) {
            DGNode* n = _children[0]->findLeader();
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
    #ifndef _NLOG_
        logFile << "name=" << _name << endl;
    #endif
    Str2UintMap& lengthVarCntMap = dg->getLengthVarCntMap();
    for (PTNodePairList::iterator it=_lengthVarList.begin(); it!=_lengthVarList.end(); ++it) {
        const Type& type = (*it).second->getType();
        const string& name = (*it).second->getName();
        assert((type == CONST_INT || type == VAR_INT));
        if (type == CONST_INT) {
            #ifndef _NLOG_
                logFile << "const=" << (*it).second->getName() << " cnt=" << lengthVarCnt << endl;
            #endif
            _lengthVarCnt = lengthVarCnt;
            continue;
        }
        Str2UintMap::iterator jt = lengthVarCntMap.find(name);
        if (jt != lengthVarCntMap.end()) {
            #ifndef _NLOG_
                logFile << "[WARNING:DGNode::renameLengthVar] same Int Variable=" << jt->first <<  "assigned to 2 different String Variable" << endl;
            #endif 
            cout << "[WARNING:DGNode::renameLengthVar] same Int Variable=" << jt->first << "assigned to 2 different String Variable" << endl;
        }
        else {
            #ifndef _NLOG_
                logFile << "var=" << name << " cnt=" << lengthVarCnt << endl;
            #endif
            _lengthVarCnt = lengthVarCnt;
            lengthVarCntMap.insert(Str2Uint(name,lengthVarCnt));
        }
    }
    if (!_lengthVarList.empty()) ++lengthVarCnt;
    for (DGNodeList::iterator it=_children.begin(); it!=_children.end(); ++it)
        (*it)->findLeader()->renameLengthVar(lengthVarCnt);
}
