#include "ptnode.h"
#include "dg.h"
#include "kaluzaMgr.h"

extern                     KaluzaMgr*   kmgr;
static Smt*&                pt         = kmgr->getSmt();
static DG*&                dg         = kmgr->getDG();
static const size_t&       gflag      = kmgr->getGFlag();
static const Str2TypeMap&  typeMap    = kmgr->getTypeMap();
static ofstream&           logFile    = kmgr->getLogFile();
static SmtNodeQueue&        ptq        = pt->getSmtQ();
static IMPQueue&           impq       = pt->getIMPQ();
static Str2DTNodeMap&      dgMap      = pt->getDGMap();
static Str2SmtNodeListMap&  ptnodeListMap = pt->getSmtNodeListMap();

//-------------base class-----------

void SmtNode::print(const size_t& indent,size_t level) const
{
    #ifndef _NLOG_ // TODO print typestr
    cout << string( indent * level ,' ') << _name << " " << this << endl;
    #endif
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->print(indent,level+1);
}

void SmtNode::analyzeASCII() const
{
    bool nonascii = 0;
    if (_type == CONST_STRING)
        for (string::const_iterator it=_name.begin(); it!=_name.end(); ++it)
            if (*it < 0 || *it >= 128) nonascii = 1;
    if (nonascii) cout << "NON-ASCII=" << _name << endl;
    for (size_t i=0,size=_children.size(); i<size; ++i)
        _children[i]->analyzeASCII();
}

bool SmtNode::isVarStr()
{
    return (_type == VAR_STR) ? 1 : 0;
}

bool SmtNode::isConstStr()
{
    if (_name[0] == '\"') return true;
    else return false;
}

bool SmtNode::isReturnTypeStr()
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

bool SmtNode::isStrComparison()
{
    assert((_type == BOOL_EQ || _type == BOOL_NEQ));
    const Type& ltype = _children[0]->getType();
    const Type& rtype = _children[1]->getType();
    if (ltype == VAR_STRING || ltype == CONST_STRING || rtype == VAR_STRING || rtype == CONST_STRING)
        return 1;
    else
        return 0;
}

void SmtNode::writeCVC4PredVar()
{
    assert((_type == VAR_BOOL || _type == VAR_INT));
    Str2TypeMap& dgTypeMap  = dg->getTypeMap();
    vector<string>& cvc4PredList = dg->getCVC4PredList();
    vector<string>& s3PredList   = dg->getS3PredList();
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
    
    Str2SmtNodeListMap::iterator it = ptnodeListMap.find(_name);
    assert((it != ptnodeListMap.end()));
    //if (it != ptnodeListMap.end()) {
    SmtNodeList& ptnodeList = it->second;
    for (SmtNodeList::iterator jt=ptnodeList.begin(); jt!=ptnodeList.end(); ++jt) {
        if ((*jt)->_flag != gflag && ( (*jt)->_bflag == dg->getBFlag() || (*jt)->_level == 1 ) ) {
            (*jt)->_flag = gflag;
            string s = "(assert";
            (*jt)->writeCVC4PredRoot(s);
            // check if this predicate including invalid operator
            if (s[0] == 'd' && s[1] == 'i' && s[2] == 'v') continue;
            s += ")";
            cvc4PredList.push_back(s);
            s3PredList.push_back(s);
        }
    }
    //}
}

void SmtNode::writeCVC4PredRoot(string& s)
{
    if (!_children.empty()) s += " (";
    else                    s += " ";
    if (_name == "!=") s += "not (=";
    else               s += _name;
    for (SmtNodeList::iterator it=_children.begin(); it!=_children.end(); ++it) {
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
            Str2DTNodeMap::iterator jt = dgMap.find((*it)->_name);
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

void SmtNode::addChild(SmtNode* n)
{
    _children.push_back(n);
}

const SmtNodeList& SmtNode::getChildren() const
{
    return _children;
}

void SmtNode::mergeNotEquivalence()
{
    // base case
    for (SmtNodeList::iterator it = _children.begin(); it != _children.end(); ++it) {
        if ( (*it)->_name == "not" ) {
            assert( ((*it)->_children.size() == 1) );
            SmtNode*  n  = 0;
            SmtNode*& gs = (*it)->_children[0];
            const string& gsName = gs->_name;
            if (gsName == "not") {
                assert( (gs->_children.size() == 1) );
                n = gs->_children[0];
            }
            else if (gsName == "and") {
                n = new SmtOrNode();
                for (size_t i = 0, size = gs->_children.size(); i < size; ++i) {
                    SmtNode* notNode = new SmtNotNode();
                    notNode->addChild(gs->_children[i]);
                    n->addChild(notNode);
                }
            }
            else if (gsName == "or") {
                n = new SmtAndNode();
                for (size_t i = 0, size = gs->_children.size(); i < size; ++i) {
                    SmtNode* notNode = new SmtNotNode();
                    notNode->addChild(gs->_children[i]);
                    n->addChild(notNode);
                }
            }
            else if (gsName == "=") {
                n = new SmtNotEqNode();
                n->_children = gs->_children;
            }
            else if (gsName == "ite") {
                assert( (gs->_children.size() == 3) );
                n = new SmtIteNode();
                n->addChild(gs->_children[0]);
                for (size_t i = 1; i < 3; ++i) {
                    SmtNode* notNode = new SmtNotNode();
                    notNode->addChild(gs->_children[i]);
                    n->addChild(notNode);
                }
            }
            else {
                assert( (gsName == "str.in.re") );
                n = new SmtStrNotInReNode();
                n->_children = gs->_children;
            }
            _children.erase(it);
            _children.insert(it,n);
        }
    }
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->mergeNotEquivalence()
}

///////////////////////////////// Build Dependency Tree //////////////////////////////////////////////////

DTNode* SmtNdoe::buildDT()
{
    _flag = gflag;
    return 0;
}

DTNode* SmtConstStrNode::buildDT()
{
    _flag = gflag;
    DTNode* n = new DTNode(kmgr->getNewNodeName(),_name);
    return n;
}

DTNode* SmtVarStrNode::buildDT(Str2DTNodeMap& dtMap)
{
    _flag = gflag;
    Str2DTNodeMap::iterator it = dtMap.find(_name);
    if (it != dgMap.end()) {
        return it->second->findLeader();
    }
    else {
        DTNode* n = new DTNode(_name);
        dtMap.insert(Str2DTNode(_name,n));
        return n;
    }
}

DTNode* SmtEqNode::buildDT()
{
    assert((_children.size() == 2));
    _flag = gflag;
    DTNode* left  = _children[0]->buildDT();
    DTNode* right = _children[1]->buildDT();

    if (!left && !right) return 0;
    else if (left && !right) {
        const string&  LName = left->getName();
        const SmtType& RType = right->getType();
        if (LName == "str.len") {
            assert( (RType == VAR_INT || RType == CONST_INT) );
            left->addLengthVar(SmtNodePair(this,right));
        }
        else if  (LName == "=" || LName == "!=") {
            assert((_children[1]->getType() == VAR_BOOL));
            left->addAssertion(SmtNodePair(this,_children[1]));
        }
        else {
            cout << "[WARNING:SmtEqNode::buildDT] invalid right child name=" << LName << endl;
        }
        return 0;
    }
    else if (!left && right) {
        const string& RName = _children[1]->getName();
        if (RName == "str.len") {
            assert((_children[0]->getType() == VAR_INT || _children[0]->getType() == CONST_INT));
            right->addLengthVar(SmtNodePair(this,_children[0]));
        }
        else if  (RName == "=" || RName == "!=") {
            assert((_children[0]->getType() == VAR_BOOL));
            right->addAssertion(SmtNodePair(this,_children[0]));
        }
        else {
            cout << "[WARNING:SmtEqNode::buildDT] invalid left child name=" << RName << endl;
        }
        return 0;
    }
    else {
        string newLName = pt->getNewNodeName();
        string newRName = pt->getNewNodeName();
        DTNode* newLNode = new DTNode(newLName,left);
        DTNode* newRNode = new DTNode(newRName,right);
        left->clearChildren();
        right->clearChildren();
        if (left->isStrVar()) {
            right->setLeader(left);
            left->setType(AUT_INTER);
            left->addChild(newLNode);
            left->addChild(newRNode);
            return left;
        }
        else if(right->isStrVar()) {
            left->setLeader(right);
            right->setType(AUT_INTER);
            right->addChild(newLNode);
            right->addChild(newRNode);
            return right; 
        }
        else {
            cout << "[WARNING:SmtEqNode::buildDT] \"=\" two operands are not VAR_STRING"
                 << " n1=" << left->getName() << " type=" << left->getTypeString()
                 << " n2=" << right->getName() << " type=" << right->getTypeString()
                 << endl;
        }
    }
}

DTNode* SmtNotEqNode::buildDT()
{
    assert((_children.size() == 2));
    _bflag = gflag;
    DTNode* left  = _children[0]->buildDT();
    DTNode* right = _children[1]->buildDT();
    
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
    DTNode* newCNode = new DTNode(newCName,AUT_COMPLE);
    if (rtype == CONST_STRING) {
        #ifndef _NLOG_
            logFile << " set left=" << left->getName() << " to AUT_INTER" << endl;
        #endif
        newCNode->addChild(right);
        DTNode* newNode = new DTNode(newName,left);
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
        DTNode* newNode = new DTNode(newName,right);
        right->clearChildren();
        right->setType(AUT_INTER);
        right->addChild(newCNode);
        right->addChild(newNode);
        return right;
    }
}

///////////////////////////////// Bool Type //////////////////////////////////////////////////
DTNode* SmtAndNode::buildDT()
{
    _flag = gflag;
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->buildDT();
    return 0;
}

DTNode* SmtOrNode::buildDT()
{
    _flag = gflag;
    _children[_branch]->buildDT();
    return 0;
}

DTNode* SmtIteNode::buildDT()
{
    _flag = gflag;
    assert((_children.size() == 3));
    _children[_branch]->buildDT();
    return 0;
}

///////////////////////////////// String Type //////////////////////////////////////////////////
DTNode* SmtStrNode::buildDT()
{
    _flag = gflag;
    DTNode* n = new DTNode(kmgr->getNewNodeName(),_dtype);
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        DTNode* cur = _children[i]->buildDT();
        assert(cur);
        cur->setNotSink();
        n->addChild(cur);
    }
    return n;
}

DTNode* SmtStrLenNode::buildDT()
{
    //FIXME
    assert((_children.size() == 1));
    _flag = gflag;
    return _children[0]->buildDT();
}

DTNode* SmtStrInReNode::buildDT()
{
    assert((_children.size() == 2));
    _flag = gflag;
    DTNode* left  = _children[0]->buildDT();
    DTNode* right = _children[1]->buildDT();
    
    assert((left && right));
    DTNode* n = new DTNode(kmgr->getNewNodeName(),left);
    right->setNotSink();
    left->setType(INTER);
    left->clearChildren();
    left->addChild(n);
    left->addChild(right);
    
    return left;
}

DTNode* SmtStrNotInReNode::buildDT()
{
    assert((_children.size() == 2));
    _bflag = gflag;
    DTNode* left  = _children[0]->buildDT();
    DTNode* right = _children[1]->buildDT();
    
    assert((left && right));
    string newCName = pt->getNewNodeName();
    string newLName = pt->getNewNodeName();
    DTNode* newLNode = new DTNode(newLName,left);
    DTNode* newCNode = new DTNode(newCName,COMPLE);
    right->setNotSink();
    newCNode->addChild(right);
    left->setType(INTER);
    left->clearChildren();
    left->addChild(newLNode);
    left->addChild(newCNode);
    return left;
}

DTNode* SmtStrToReNode::buildDT()
{
    assert((_children.size() == 1));
    _flag = gflag;
    return _children[0]->buildDT();
}

///////////////////////////////// Build SmtNodeList Map //////////////////////////////////////////////////
void SmtNode::buildSmtNodeListMap(Str2SmtNodeListMap& m, SmtSmtNode* root) const
{
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->buildSmtNodeListMap(m,root);
}

void SmtVarBoolNode::buildSmtNodeListMap(Str2SmtNodeListMap& m, SmtNode* root) const
{
    Str2SmtNodeListMap::iterator it = m.find(_name);
    assert( (it != m.end()) );
    it->second.push_back(root);
}

void SmtVarIntNode::buildSmtNodeListMap(Str2SmtNodeListMap& m, SmtNode* root) const
{
    Str2SmtNodeListMap::iterator it = m.find(_name);
    assert( (it != m.end()) );
    it->second.push_back(root);
}

void SmtEqNode::buildSmtNodeListMap(Str2SmtNodeListMap& m, SmtNode* root) const
{
    assert( (_children.size() == 2) );
    const string&  LName = _children[0]->getName();
    const string&  RName = _children[1]->getName();
    const SmtType& LType = _children[0]->getType();
    const SmtType& RType = _children[1]->getType();
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
        for (SmtNodeList::iterator it=_children.begin(); it!=_children.end(); ++it)
            (*it)->buildSmtNodeListMap(m,root);
    }
}

void SmtNotEqNode::buildSmtNodeListMap(Str2SmtNodeListMap& m, SmtNode* root) const
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
        for (SmtNodeList::iterator it=_children.begin(); it!=_children.end(); ++it)
            (*it)->buildSmtNodeListMap(m,root);
    }
}

///////////////////////////////// Build Branching List //////////////////////////////////////////////////
void SmtNode::buildBranchList(SmtNodeList& bList)
{
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->buildBranchList(bList);
}

void SmtOrNode::buildBranchList(SmtNodeList& bList)
{
    bList.push_back(this);
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->buildBranchList(bList);
}

void SmtIteNode::buildBranchList(SmtNodeList& bList)
{
    bList.push_back(this);
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->buildBranchList(bList);
}

