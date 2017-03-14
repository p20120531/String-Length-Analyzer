#include "ptnode.h"
#include "dg.h"
#include "kaluzaMgr.h"

extern                     KaluzaMgr*   kmgr;
static PT*&                pt         = kmgr->getPT();
static const size_t&       gflag      = kmgr->getGFlag();
static const Str2PTNodeMap& ptnodeMap = kmgr->getPTNodeMap();
static ofstream&           logFile    = kmgr->getLogFile();
static PTNodeQueue&        ptq        = pt->getPTQ();
static Str2DGNodeMap&      dgMap      = pt->getDGMap();
static PTNode2PTNodeListMap& ptnodeListMap = pt->getPTNodeListMap();
//-------------base class-----------

void PTNode::print(const size_t& indent,size_t level) const
{
    //logFile << string( indent * level ,' ') << _name << endl;
    logFile << string( indent * level ,' ') << _name << " " << this << endl;
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->print(indent,level+1);
}

void PTNode::analyze(bool& iteDVarLegal, bool& iteCLevel1, bool& strinreRLevel1, bool& strninreRLevel1, bool& streqRLevel1, bool& strneqRLevel1, bool& strlenRLevel2, bool& andCLevel2, bool& ornexist, bool& strinreLCSV, bool& strninreLCSV, bool& streqLCSV, bool& strneqLCSV, size_t& strlenCnt, size_t& strlenEqCnt, bool& streqBothSV, bool& strneqBothSV, bool& strneqOneConst, int cLevel, int rLevel) const
{
    // cLevel : cumulated level
    // rLevel : reset level
    if (_name == "ite") {
        if (cLevel != 1) iteCLevel1 = 0;
        rLevel = -1;
        Str2PTNodeMap::const_iterator it = ptnodeMap.find(_children[0]->_name);
        if (it != ptnodeMap.end()) {
            if ((it->second)->getType() != VAR_BOOL) {
                iteDVarLegal = 0;
            }
        }
    }
    else if (_name == "str.in.re") {
        if (rLevel != 1) strinreRLevel1 = 0;
        if (!_children[0]->isVarStr()) strinreLCSV = 0;
    }
    else if (_name == "str.nin.re") {
        if (rLevel != 1) strninreRLevel1 = 0;
        if (!_children[0]->isVarStr()) strninreLCSV = 0;
    }
    else if (_name == "str.len") {
        ++strlenCnt;
        if (rLevel != 2) strlenRLevel2 = 0;
    }
    else if (_name == "=") {
        if ( _children[0]->isReturnTypeStr() || _children[1]->isReturnTypeStr() ) {
            if (rLevel != 1) streqRLevel1 = 0;
            if (!_children[0]->isVarStr()) streqLCSV = 0;
            if (_children[0]->isVarStr() && _children[1]->isVarStr()) streqBothSV = 1;
        }
        for (size_t i = 0, size = _children.size(); i < size; ++i)
            if (_children[i]->_name == "str.len") ++strlenEqCnt;
    }
    else if (_name == "!=") {
        if ( _children[0]->isReturnTypeStr() || _children[1]->isReturnTypeStr()) {
            if (rLevel != 1) strneqRLevel1 = 0;
            if (!_children[0]->isVarStr()) strneqLCSV = 0;
            if (_children[0]->isVarStr() && _children[1]->isVarStr()) strneqBothSV = 1;
            if (!_children[0]->isConstStr() && !_children[1]->isConstStr()) strneqOneConst = 0;
        }
    }
    else if (_name == "and" && cLevel != 2) andCLevel2 = 0; 
    else if (_name == "or") ornexist = 0;
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->analyze(
            iteDVarLegal,iteCLevel1,strinreRLevel1,strninreRLevel1,
            streqRLevel1,strneqRLevel1,strlenRLevel2,andCLevel2,ornexist,
            strinreLCSV,strninreLCSV,streqLCSV,strneqLCSV,strlenCnt,strlenEqCnt,
            streqBothSV,strneqBothSV,strneqOneConst,cLevel+1,rLevel+1);
}

bool PTNode::isVarStr()
{
    Str2PTNodeMap::const_iterator it = ptnodeMap.find(_name);
    if (it != ptnodeMap.end()) {
        if (it->second->getType() == VAR_STRING)
            return true;
        else
            return false;
    }
    else {
        return false;
    }
}

bool PTNode::isConstStr()
{
    if (_name[0] == '\"') return true;
    else return false;
}

bool PTNode::isReturnTypeStr()
{
    if (_name == "str.++") return true;
    else {
        Str2PTNodeMap::const_iterator it = ptnodeMap.find(_name);
        if (it != ptnodeMap.end()) {
            if (it->second->getType() == VAR_STRING)
                return true;
            else
                return false;
        }
        else 
            return false;
    }
}

void PTNode::setLevel(size_t level)
{
    _level = level;
    for (PTNodeList::iterator it = _children.begin(); it != _children.end(); ++it) {
        (*it)->setLevel(level+1);
    }
}

void PTNode::lcTraversal(ofstream& outFile,const Str2UintMap& dgIntVarMap) const
{
    if (_children.empty()) {
        outFile << " " << _name;
        return;
    }
        
    if (_name == "!=") {
        assert((_children.size() == 1));
        outFile << " (not (=";
        _children[0]->lcTraversal(outFile,dgIntVarMap);
        outFile << "))";
    }
    else if (_name == "str.len") {
        assert((_children.size() == 1));
        Str2UintMap::const_iterator it=dgIntVarMap.find(_children[0]->_name);
        assert((it!=dgIntVarMap.end()));
        outFile << " n" << it->second;
    }
    else {
        outFile << " (" << _name;
        for (PTNodeList::const_iterator it=_children.begin();it!=_children.end();++it)
            (*it)->lcTraversal(outFile,dgIntVarMap);
        outFile << ")";
    }
}

void PTNode::writeCVC4Pred(Str2TypeMap& typeMap,vector<string>& cvc4PredList,const size_t& bflag)
{
}

void PTNode::addChild(PTNode* n)
{
    _children.push_back(n);
}

const PTNodeList& PTNode::getChildren() const
{
    return _children;
}

//----------------print--------------
void PTVarIntNode::print(const size_t& indent , size_t level) const 
{
    logFile << string( indent * level ,' ')
            << _name
            << " " << this
            << " VAR_INT" << endl;
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->print(indent,level+1);
}

void PTVarBoolNode::print(const size_t& indent , size_t level) const 
{
    logFile << string( indent * level ,' ')
            << _name
            << " " << this
            << " VAR_BOOL" << endl;
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->print(indent,level+1);
}

void PTVarStringNode::print(const size_t& indent , size_t level) const 
{
    logFile << string( indent * level ,' ')
            << _name
            << " " << this
            << " VAR_STRING" << endl;
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->print(indent,level+1);
}

void PTConstIntNode::print(const size_t& indent , size_t level) const 
{
    logFile << string( indent * level ,' ')
            << _name
            << " " << this
            << " CONST_INT" << endl;
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->print(indent,level+1);
}

void PTConstBoolNode::print(const size_t& indent , size_t level) const 
{
    logFile << string( indent * level ,' ')
            << _name
            << " " << this
            << " CONST_BOOL" << endl;
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->print(indent,level+1);
}

void PTConstStringNode::print(const size_t& indent , size_t level) const 
{
    logFile << string( indent * level ,' ')
            << _name
            << " " << this
            << " CONST_STRING" << endl;
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->print(indent,level+1);
}

void PTIteNode::print(const size_t& indent , size_t level) const
{
    logFile << string( indent * level ,' ')
            << _name
            << " " << this
            << " ITE" << endl;
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->print(indent,level+1);
}

//-------------------buildDG------------------

DGNode* PTVarIntNode::buildDG()
{
    _bflag = gflag;
    #ifndef _NLOG_
        logFile << _name << " VAR_INT" << endl;
    #endif
    return 0;
}

DGNode* PTVarBoolNode::buildDG()
{
    _bflag = gflag;
    #ifndef _NLOG_
        logFile << _name << " VAR_BOOL" << endl;
    #endif
    return 0;
}

DGNode* PTVarStringNode::buildDG()
{
    _bflag = gflag;
    #ifndef _NLOG_
        logFile << _name << " VAR_STRING";
    #endif
    Str2DGNodeMap::iterator it = dgMap.find(_name);
    if (it != dgMap.end()) {
        #ifndef _NLOG_
            logFile << " => already exist ,"
                    << " leader=" << it->second->findLeader()->getName() 
                    << " type=" << it->second->findLeader()->getType() 
                    << endl;
        #endif
        return it->second->findLeader();
    }
    else {
        DGNode* newNode = new DGNode(_name);
        dgMap.insert(Str2DGNode(_name,newNode));
        #ifndef _NLOG_
            logFile << " => create new node"
                    << " name=" << newNode->getName() 
                    << endl;
        #endif
        return newNode;
    }
}

DGNode* PTConstIntNode::buildDG()
{
    _bflag = gflag;
    #ifndef _NLOG_
        logFile << _name << " CONST_INT" << endl;
    #endif
    return 0;
}

DGNode* PTConstBoolNode::buildDG()
{
    _bflag = gflag;
    #ifndef _NLOG_
        logFile << _name << " CONST_BOOL" << endl;
    #endif
    return 0;
}

DGNode* PTConstStringNode::buildDG()
{
    _bflag = gflag;
    string newName = pt->getNewNodeName();
    #ifndef _NLOG_
        logFile << _name << " CONST_STRING => create new node name=" << newName << endl;
    #endif
    DGNode* newNode = new DGNode(newName,_name);
    dgMap.insert(Str2DGNode(newNode->getName(),newNode));
    return newNode;
}

//----------------bool type-------------------

DGNode* PTNotNode::buildDG()
{
    _bflag = gflag;
    // irrelavent to automaton operation
    #ifndef _NLOG_
        logFile << _name << " => return 0" << endl;
    #endif
    return 0;
}

DGNode* PTEqNode::buildDG()
{
    assert((_children.size() == 2));
    _bflag = gflag;
    DGNode* left  = _children[0]->buildDG();
    DGNode* right = _children[1]->buildDG();
    
    #ifndef _NLOG_
        logFile << _name << " => left=";
        if (!left) logFile << "0";
        else {
            logFile << left->getName() << left->getTypeString();
        }
        logFile << " right=";
        if (!right) logFile<< "0" << endl;
        else {
            logFile << right->getName() << right->getTypeString() << endl;
        }
    #endif
    
    if (!left || !right) {
        const string& lname = _children[0]->getName();
        const string& rname = _children[1]->getName();

        if (lname == "str.len")
            left->addLengthVar(_children[1]);
        else if (rname == "str.len")
            right->addLengthVar(_children[0]);
        else if (lname == "==" || lname == "!=")
            left->addIMP(IMP(_children[1],1));
        else if (rname == "==" || rname == "!=")
            right->addIMP(IMP(_children[0],1));
        return 0;
    }
    else {
        string newLName = pt->getNewNodeName();
        string newRName = pt->getNewNodeName();
        #ifndef _NLOG_
            logFile << " newLName=" << newLName
                    << " newRName=" << newRName;
        #endif
        DGNode* newLNode = new DGNode(newLName,left);
        DGNode* newRNode = new DGNode(newRName,right);
        left->clearChildren();
        right->clearChildren();
        if (left->isStrVar()) {
            #ifndef _NLOG_
                logFile << " set right=" << right->getName() << " leader=left" << endl;
            #endif
            right->setLeader(left);
            left->setType(AUT_INTER);
            left->addChild(newLNode);
            left->addChild(newRNode);
            return left;
        }
        else if(right->isStrVar()) {
            #ifndef _NLOG_
                logFile << " set left=" << left->getName() << " leader=right" << endl;
            #endif
            left->setLeader(right);
            right->setType(AUT_INTER);
            right->addChild(newLNode);
            right->addChild(newRNode);
            return right; 
        }
        else {
            #ifndef _NLOG_
                logFile << " [WARNING05]: [=] two operands are not VAR_STRING"
                        << " n1=" << left->getName() << " type=" << left->getTypeString()
                        << " n2=" << right->getName() << " type=" << right->getTypeString()
                        << endl;
            #endif
            cout << "[WARNING05]: [=] two operands are not VAR_STRING"
                 << " n1=" << left->getName() << " type=" << left->getTypeString()
                 << " n2=" << right->getName() << " type=" << right->getTypeString()
                 << endl;
        }
    }
}

DGNode* PTNotEqNode::buildDG()
{
    assert((_children.size() == 2));
    _bflag = gflag;
    DGNode* left  = _children[0]->buildDG();
    DGNode* right = _children[1]->buildDG();
    
    #ifndef _NLOG_
        logFile << _name << " => left=";
        if (!left) logFile << "0";
        else       logFile << left->getName();
        logFile << " right=";
        if (!right) logFile<< "0";
        else        logFile<< right->getName();
    #endif

    if (!left || !right) {
        #ifndef _NLOG_
        logFile << endl;
        #endif
        return 0;
    }
    
    const Type& ltype = left->getType();
    const Type& rtype = right->getType();
    
    assert((ltype == CONST_STRING || rtype == CONST_STRING));

    string newCName = pt->getNewNodeName();
    string newName  = pt->getNewNodeName();
    DGNode* newCNode = new DGNode(newCName,AUT_COMPLE);
    if (rtype == CONST_STRING) {
        #ifndef _NLOG_
            logFile << " set left=" << left->getName() << " to AUT_INTER" << endl;
        #endif
        newCNode->addChild(right);
        DGNode* newNode = new DGNode(newName,left);
        left->clearChildren();
        left->setType(AUT_INTER);
        left->addChild(newNode);
        left->addChild(newCNode);
        return left;
    }
    else {
        #ifndef _NLOG_
            logFile << " set right=" << right->getName() << " to AUT_INTER" << endl;
        #endif
        newCNode->addChild(left);
        DGNode* newNode = new DGNode(newName,right);
        right->clearChildren();
        right->setType(AUT_INTER);
        right->addChild(newCNode);
        right->addChild(newNode);
        return right;
    }
}

DGNode* PTAndNode::buildDG()
{
    _bflag = gflag;
    #ifndef _NLOG_
        logFile << _name << " => return 0" << endl;
    #endif
    return 0;
}

DGNode* PTOrNode::buildDG()
{
    _bflag = gflag;
    #ifndef _NLOG_
        logFile << _name << " => return 0" << endl;
    #endif
    return 0;
}

DGNode* PTIteNode::buildDG()
{
    _bflag = gflag;
    assert((_children.size() == 3));
    #ifndef _NLOG_
        logFile << _name << " =>"
                << " push " << _children[1]->getName()
                << " push " << _children[2]->getName() 
                << endl;
    #endif
    ptq.push(_children[1]);
    ptq.push(_children[2]);
    return 0;
}

DGNode* PTLTNode::buildDG()
{
    _bflag = gflag;
    return 0;
}

DGNode* PTLTOEQNode::buildDG()
{
    _bflag = gflag;
    return 0;
}

DGNode* PTMTNode::buildDG()
{
    _bflag = gflag;
    return 0;
}

DGNode* PTMTOEQNode::buildDG()
{
    _bflag = gflag;
    return 0;
}

//-----------------int type-------------------

DGNode* PTPlusNode::buildDG()
{
    _bflag = gflag;
    return 0;
}

DGNode* PTMinusNode::buildDG()
{
    _bflag = gflag;
    return 0;
}

DGNode* PTDivNode::buildDG()
{
    _bflag = gflag;
    return 0;
}
//-----------------string type-------------------

DGNode* PTStrConcateNode::buildDG()
{
    _bflag = gflag;
    string newName = pt->getNewNodeName();
    #ifndef _NLOG_
        logFile << _name << " => create new node name=" << newName << endl;
    #endif
    DGNode* newNode = new DGNode(newName,AUT_CONCATE);
    //dgMap.insert(Str2DGNode(newName,newNode));
    for (PTNodeList::iterator it=_children.begin(); it!=_children.end(); ++it) {
        DGNode* cur = (*it)->buildDG();
        assert(cur);
        cur->setNotSink();
        newNode->addChild(cur);
    }
    return newNode;
}

DGNode* PTStrLenNode::buildDG()
{
    //FIXME
    assert((_children.size() == 1));
    _bflag = gflag;
    return _children[0]->buildDG();
}

DGNode* PTStrInReNode::buildDG()
{
    assert((_children.size() == 2));
    _bflag = gflag;
    DGNode* left  = _children[0]->buildDG();
    DGNode* right = _children[1]->buildDG();
    
    assert((left && right));
    string newLName = pt->getNewNodeName();
    #ifndef _NLOG_
        logFile << _name << " => left=" <<  left->getName()
                         << " right=" << right->getName()
                         << " newLName=" << newLName 
                         << endl;
    #endif
    DGNode* newNode = new DGNode(newLName,left);
    //dgMap.insert(Str2DGNode(newLName,newNode));
    right->setNotSink();
    left->setType(AUT_INTER);
    left->clearChildren();
    left->addChild(newNode);
    left->addChild(right);
    
    return left;
}

DGNode* PTStrNotInReNode::buildDG()
{
    assert((_children.size() == 2));
    _bflag = gflag;
    DGNode* left  = _children[0]->buildDG();
    DGNode* right = _children[1]->buildDG();
    
    assert((left && right));
    string newCName = pt->getNewNodeName();
    string newLName = pt->getNewNodeName();
    #ifndef _NLOG_
        logFile << _name << " => left=" <<  left->getName()
                         << " right=" << right->getName() 
                         << " newLName=" << newLName
                         << " comple=" << newCName << endl;
    #endif
    DGNode* newLNode = new DGNode(newLName,left);
    DGNode* newCNode = new DGNode(newCName,AUT_COMPLE);
    //dgMap.insert(Str2DGNode(newLName,newLNode));
    //dgMap.insert(Str2DGNode(newCName,newCNode));
    right->setNotSink();
    newCNode->addChild(right);
    left->setType(AUT_INTER);
    left->clearChildren();
    left->addChild(newLNode);
    left->addChild(newCNode);
    return left;
}

DGNode* PTStrReplaceNode::buildDG()
{
    _bflag = gflag;
    string newName = pt->getNewNodeName();
    #ifndef _NLOG_
        logFile << _name << " => create new node name=" << newName << endl;
    #endif
    assert((_children.size() == 3));
    DGNode* newNode = new DGNode(newName,AUT_REPLACE);
    //dgMap.insert(Str2DGNode(newName,newNode));
    for (PTNodeList::iterator it=_children.begin(); it!=_children.end(); ++it) {
        DGNode* cur = (*it)->buildDG();
        assert(cur);
        cur->setNotSink();
        newNode->addChild(cur);
    }
    return newNode;
}

DGNode* PTStrToReNode::buildDG()
{
    assert((_children.size() == 1));
    _bflag = gflag;
    #ifndef _DEBUG_
        logFile << _name << " => return child=" << _children[0]->getName() << endl;
    #endif
    return _children[0]->buildDG();
}

//-----------------regex type-------------------

DGNode* PTReConcateNode::buildDG()
{
    _bflag = gflag;
    string newName = pt->getNewNodeName();
    #ifndef _NLOG_
        logFile << _name << " => create new node name=" << newName << endl;
    #endif
    DGNode* newNode = new DGNode(newName,AUT_CONCATE);
    //dgMap.insert(Str2DGNode(newName,newNode));
    for (PTNodeList::iterator it=_children.begin(); it!=_children.end(); ++it) {
        DGNode* cur = (*it)->buildDG();
        assert(cur);
        cur->setNotSink();
        newNode->addChild(cur);
    }
    return newNode;
}

DGNode* PTReUnionNode::buildDG()
{
    _bflag = gflag;
    string newName = pt->getNewNodeName();
    #ifndef _NLOG_
        logFile << _name << " => create new node name=" << newName << endl;
    #endif
    DGNode* newNode = new DGNode(newName,AUT_UNION);
    //dgMap.insert(Str2DGNode(newName,newNode));
    for (PTNodeList::iterator it=_children.begin(); it!=_children.end(); ++it) {
        DGNode* cur = (*it)->buildDG();
        assert(cur);
        cur->setNotSink();
        newNode->addChild(cur);
    }
    return newNode;
}

DGNode* PTReInterNode::buildDG()
{
    _bflag = gflag;
    string newName = pt->getNewNodeName();
    #ifndef _NLOG_
        logFile << _name << " => create new node name=" << newName << endl;
    #endif
    DGNode* newNode = new DGNode(newName,AUT_INTER);
    //dgMap.insert(Str2DGNode(newName,newNode));
    for (PTNodeList::iterator it=_children.begin(); it!=_children.end(); ++it) {
        DGNode* cur = (*it)->buildDG();
        assert(cur);
        cur->setNotSink();
        newNode->addChild(cur);
    }
    return newNode;
}

//-------------------buildPTNodeListMap------------------
void PTNode::buildPTNodeListMap(PTNode* root)
{
    return;
}
void PTVarIntNode::buildPTNodeListMap(PTNode* root)
{
    PTNode2PTNodeListMap::iterator it = ptnodeListMap.find(this);
    assert((it!=ptnodeListMap.end()));
    it->second.push_back(root);
}
void PTVarBoolNode::buildPTNodeListMap(PTNode* root)
{
    PTNode2PTNodeListMap::iterator it = ptnodeListMap.find(this);
    assert((it!=ptnodeListMap.end()));
    it->second.push_back(root);
}
void PTNotNode::buildPTNodeListMap(PTNode* root)
{
    assert((_children.size()==1));
    _children[0]->buildPTNodeListMap(root);
}
void PTEqNode::buildPTNodeListMap(PTNode* root)
{
    for (PTNodeList::iterator it=_children.begin(); it!=_children.end(); ++it)
        (*it)->buildPTNodeListMap(root);
}
void PTNotEqNode::buildPTNodeListMap(PTNode* root)
{
    for (PTNodeList::iterator it=_children.begin(); it!=_children.end(); ++it)
        (*it)->buildPTNodeListMap(root);
}
void PTAndNode::buildPTNodeListMap(PTNode* root)
{
    for (PTNodeList::iterator it=_children.begin(); it!=_children.end(); ++it)
        (*it)->buildPTNodeListMap(root);
}
void PTOrNode::buildPTNodeListMap(PTNode* root)
{
    for (PTNodeList::iterator it=_children.begin(); it!=_children.end(); ++it)
        (*it)->buildPTNodeListMap(root);
}
void PTLTNode::buildPTNodeListMap(PTNode* root)
{
    for (PTNodeList::iterator it=_children.begin(); it!=_children.end(); ++it)
        (*it)->buildPTNodeListMap(root);
}
void PTLTOEQNode::buildPTNodeListMap(PTNode* root)
{
    for (PTNodeList::iterator it=_children.begin(); it!=_children.end(); ++it)
        (*it)->buildPTNodeListMap(root);
}
void PTMTNode::buildPTNodeListMap(PTNode* root)
{
    for (PTNodeList::iterator it=_children.begin(); it!=_children.end(); ++it)
        (*it)->buildPTNodeListMap(root);
}
void PTMTOEQNode::buildPTNodeListMap(PTNode* root)
{
    for (PTNodeList::iterator it=_children.begin(); it!=_children.end(); ++it)
        (*it)->buildPTNodeListMap(root);
}
void PTPlusNode::buildPTNodeListMap(PTNode* root)
{
    for (PTNodeList::iterator it=_children.begin(); it!=_children.end(); ++it)
        (*it)->buildPTNodeListMap(root);
}
void PTMinusNode::buildPTNodeListMap(PTNode* root)
{
    for (PTNodeList::iterator it=_children.begin(); it!=_children.end(); ++it)
        (*it)->buildPTNodeListMap(root);
}
void PTDivNode::buildPTNodeListMap(PTNode* root)
{
    for (PTNodeList::iterator it=_children.begin(); it!=_children.end(); ++it)
        (*it)->buildPTNodeListMap(root);
}
