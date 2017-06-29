#include "ptnode.h"
#include "dg.h"
#include "kaluzaMgr.h"

extern                     KaluzaMgr*   kmgr;
static PT*&                pt         = kmgr->getPT();
static DG*&                dg         = kmgr->getDG();
static const size_t&       gflag      = kmgr->getGFlag();
static const Str2TypeMap&  typeMap    = kmgr->getTypeMap();
static ofstream&           logFile    = kmgr->getLogFile();
static PTNodeQueue&        ptq        = pt->getPTQ();
static IMPQueue&           impq       = pt->getIMPQ();
static Str2DGNodeMap&      dgMap      = pt->getDGMap();
static Str2PTNodeListMap&  ptnodeListMap = pt->getPTNodeListMap();

//-------------base class-----------

void PTNode::print(const size_t& indent,size_t level) const
{
    #ifndef _NLOG_
    logFile << string( indent * level ,' ') << _name << " " << this << endl;
    #endif
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->print(indent,level+1);
}

void PTNode::analyzeASCII() const
{
    bool nonascii = 0;
    if (_type == CONST_STRING)
        for (string::const_iterator it=_name.begin(); it!=_name.end(); ++it)
            if (*it < 0 || *it >= 128) nonascii = 1;
    if (nonascii) cout << "NON-ASCII=" << _name << endl;
    for (size_t i=0,size=_children.size(); i<size; ++i)
        _children[i]->analyzeASCII();
}

void PTNode::analyze(bool& iteDVarLegal, bool& iteCLevel1, bool& iteChildNotAnd, bool& strinreRLevel1, bool& strninreRLevel1, bool& streqRLevel1, bool& strneqRLevel1, bool& strlenRLevel2, bool& andCLevel2, bool& ornexist, bool& strinreLCSV, bool& strninreLCSV, bool& streqLCSV, bool& strneqLCSV, size_t& strlenCnt, size_t& strlenEqCnt, bool& streqBothSV, bool& strneqBothSV, bool& strneqOneConst, bool& streqBothSC, int cLevel, int rLevel, bool& strinreReConcateMT2, bool& strConcateMT2, size_t& strinreReConcateCnt, size_t& strninreReConcateCnt, size_t& reConcateCnt) const
{
    // cLevel : cumulated level
    // rLevel : reset level
    if (_name == "ite") {
        if (cLevel != 1) iteCLevel1 = 0;
        if (_children[1]->_name != "and" || _children[2]->_name != "and") iteChildNotAnd = 1;
        rLevel = -1;
        Str2TypeMap::const_iterator it = typeMap.find(_children[0]->_name);
        if (it != typeMap.end()) {
            if (it->second != VAR_BOOL) {
                iteDVarLegal = 0;
            }
        }
    }
    else if (_name == "str.in.re") {
        //cout << "left=" << _children[0]->_name <<  " right=" << _children[1]->_name << endl;
        if (rLevel != 1) strinreRLevel1 = 0;
        if (!_children[0]->isVarStr()) strinreLCSV = 0;
        if (_children[1]->_name == "re.++") {
            ++strinreReConcateCnt;
            if (_children[1]->_children.size() > 2)
                strinreReConcateMT2 = 1;
        }
    }
    else if (_name == "str.nin.re") {
        //cout << "left=" << _children[0]->_name <<  " right=" << _children[1]->_name << endl;
        if (rLevel != 1) strninreRLevel1 = 0;
        if (!_children[0]->isVarStr()) strninreLCSV = 0;
        if (_children[1]->_name == "re.++") {
            ++strninreReConcateCnt;
        }
    }
    else if (_name == "re.++") {
        ++reConcateCnt;
    }
    else if (_name == "str.++") {
        if (_children.size() > 2)
            strConcateMT2 = 1;
    }
    else if (_name == "str.len") {
        ++strlenCnt;
        if (rLevel != 2) strlenRLevel2 = 0;
    }
    else if (_name == "str.to.re") {
        assert ((_children.size() == 1));
        assert ((_children[0]->getType() == CONST_STRING));
    }
    else if (_name == "=") {
        if ( _children[0]->isReturnTypeStr() || _children[1]->isReturnTypeStr() ) {
            if (rLevel != 1) streqRLevel1 = 0;
            if (!_children[0]->isVarStr()) streqLCSV = 0;
            if (_children[0]->isVarStr() && _children[1]->isVarStr()) streqBothSV = 1;
            if (_children[0]->isConstStr() && _children[1]->isConstStr()) streqBothSC = 1;
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
            iteDVarLegal,iteCLevel1,iteChildNotAnd,strinreRLevel1,strninreRLevel1,
            streqRLevel1,strneqRLevel1,strlenRLevel2,andCLevel2,ornexist,
            strinreLCSV,strninreLCSV,streqLCSV,strneqLCSV,strlenCnt,strlenEqCnt,
            streqBothSV,strneqBothSV,strneqOneConst,streqBothSC,cLevel+1,rLevel+1,
            strinreReConcateMT2,strConcateMT2,strinreReConcateCnt,strninreReConcateCnt,reConcateCnt);
}

bool PTNode::isVarStr()
{
    Str2TypeMap::const_iterator it = typeMap.find(_name);
    if (it != typeMap.end()) {
        if (it->second == VAR_STRING)
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
        Str2TypeMap::const_iterator it = typeMap.find(_name);
        if (it != typeMap.end()) {
            if (it->second == VAR_STRING)
                return true;
            else
                return false;
        }
        else 
            return false;
    }
}

bool PTNode::isStrComparison()
{
    assert((_type == BOOL_EQ || _type == BOOL_NEQ));
    const Type& ltype = _children[0]->getType();
    const Type& rtype = _children[1]->getType();
    if (ltype == VAR_STRING || ltype == CONST_STRING || rtype == VAR_STRING || rtype == CONST_STRING)
        return 1;
    else
        return 0;
}

void PTNode::setLevel(size_t level)
{
    _level = level;
    for (PTNodeList::iterator it = _children.begin(); it != _children.end(); ++it) {
        (*it)->setLevel(level+1);
    }
}

void PTNode::writeCVC4PredVar()
{
    assert((_type == VAR_BOOL || _type == VAR_INT));
    Str2TypeMap& dgTypeMap  = dg->getTypeMap();
    vector<string>& cvc4PredList = dg->getCVC4PredList();
    set<string>& bvPredSet  = dg->getBVPredSet();
    set<string>& ivPredSet  = dg->getIVPredSet();
    set<string>::iterator kt;
    if (_type == VAR_BOOL) {
        kt = bvPredSet.find(_name);
        if (kt != bvPredSet.end()) return;
        else bvPredSet.insert(_name);
    }
    else {
        kt = ivPredSet.find(_name);
        if (kt != ivPredSet.end()) return;
        else ivPredSet.insert(_name);
    }

    Str2TypeMap::iterator sit = dgTypeMap.find(_name);
    if (sit == dgTypeMap.end()) {
        dgTypeMap.insert(Str2Type(_name,_type));
    }
    else {
        //FIXME
    }
    
    Str2PTNodeListMap::iterator it = ptnodeListMap.find(_name);
    assert((it != ptnodeListMap.end()));
    //if (it != ptnodeListMap.end()) {
    PTNodeList& ptnodeList = it->second;
    for (PTNodeList::iterator jt=ptnodeList.begin(); jt!=ptnodeList.end(); ++jt) {
        if ((*jt)->_flag != gflag && ( (*jt)->_bflag == dg->getBFlag() || (*jt)->_level == 1 ) ) {
            (*jt)->_flag = gflag;
            string s = "(assert";
            (*jt)->writeCVC4PredRoot(s);
            // check if this predicate including invalid operator
            if (s[0] == 'd' && s[1] == 'i' && s[2] == 'v') continue;
            s += ")";
            cvc4PredList.push_back(s);
        }
    }
    //}
}

void PTNode::writeCVC4PredRoot(string& s)
{
    if (!_children.empty()) s += " (";
    else                    s += " ";
    if (_name == "!=") s += "not (=";
    else               s += _name;
    for (PTNodeList::iterator it=_children.begin(); it!=_children.end(); ++it) {
        // eliminate predicate including div
        if (_name == "div") {
            s = "div";
        }
        const Type& type = (*it)->_type;
        if (type == VAR_INT || type == VAR_BOOL) {
            s += " " + (*it)->_name;
            (*it)->writeCVC4PredVar();
        }
        else if (type == CONST_BOOL || type == CONST_INT || type == CONST_STRING) {
            s += " " + (*it)->_name;
        }
        else if (type == VAR_STRING) {
            Str2DGNodeMap::iterator jt = dgMap.find((*it)->_name);
            assert((jt != dgMap.end()));
            s += " " + jt->second->findLeader()->getName();
        }
        else {
            (*it)->writeCVC4PredRoot(s);
        }
    }
    if (_name == "!=") s += ")";
    if (!_children.empty()) s += ")";
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
    //dgMap.insert(Str2DGNode(newNode->getName(),newNode));
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

    if (!left && !right) return 0;
    else if (left && !right) {
        const string& LName = _children[0]->getName();
        if (LName == "str.len") {
            assert((_children[1]->getType() == VAR_INT || _children[1]->getType() == CONST_INT));
            left->addLengthVar(PTNodePair(this,_children[1]));
        }
        else if  (LName == "=" || LName == "!=") {
            assert((_children[1]->getType() == VAR_BOOL));
            left->addAssertion(PTNodePair(this,_children[1]));
        }
        else {
            #ifndef _NLOG_
                logFile << "[WARNING:PTEqNode::buildDG] invalid right child name=" << LName << endl;
            #endif
            cout << "[WARNING:PTEqNode::buildDG] invalid right child name=" << LName << endl;
        }
        return 0;
    }
    else if (!left && right) {
        const string& RName = _children[1]->getName();
        if (RName == "str.len") {
            assert((_children[0]->getType() == VAR_INT || _children[0]->getType() == CONST_INT));
            right->addLengthVar(PTNodePair(this,_children[0]));
        }
        else if  (RName == "=" || RName == "!=") {
            assert((_children[0]->getType() == VAR_BOOL));
            right->addAssertion(PTNodePair(this,_children[0]));
        }
        else {
            #ifndef _NLOG_
                logFile << "[WARNING:PTEqNode::buildDG] invalid left child name=" << RName << endl;
            #endif
            cout << "[WARNING:PTEqNode::buildDG] invalid left child name=" << RName << endl;
        }
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
                logFile << " [WARNING:PTEqNode::buildDG] \"=\" two operands are not VAR_STRING"
                        << " n1=" << left->getName() << " type=" << left->getTypeString()
                        << " n2=" << right->getName() << " type=" << right->getTypeString()
                        << endl;
            #endif
            cout << "[WARNING:PTEqNode::buildDG] \"=\" two operands are not VAR_STRING"
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
    for (PTNodeList::iterator it=_children.begin(); it!=_children.end(); ++it)
        (*it)->buildDG();
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
    impq.push(IMP(_children[0],1));
    impq.push(IMP(_children[0],0));
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
        logFile << "str.in.re "
                << _name << " => left=" <<  left->getName()
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
        logFile << "str.nin.re "
                << _name << " => left=" <<  left->getName()
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
    Str2PTNodeListMap::iterator it = ptnodeListMap.find(_name);
    assert((it!=ptnodeListMap.end()));
    it->second.push_back(root);
}
void PTVarBoolNode::buildPTNodeListMap(PTNode* root)
{
    Str2PTNodeListMap::iterator it = ptnodeListMap.find(_name);
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
    const string& LName = _children[0]->getName();
    const string& RName = _children[1]->getName();
    const Type&   LType = _children[0]->getType();
    const Type&   RType = _children[1]->getType();
    if (LName == "str.len") {
        assert((RType == VAR_INT || RType == CONST_INT));
        return;
    }
    else if (RName == "str.len") {
        assert((LType == VAR_INT || LType == CONST_INT));
        return;
    }
    else if ( (LName == "=" || LName == "!=") && _children[0]->isStrComparison() ) {
        assert((RType == VAR_BOOL));
        return;
    }
    else if ( (RName == "=" || RName == "!=") && _children[1]->isStrComparison() ) {
        assert((LType == VAR_BOOL));
        return;
    }
    else {
        for (PTNodeList::iterator it=_children.begin(); it!=_children.end(); ++it)
            (*it)->buildPTNodeListMap(root);
    }
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
