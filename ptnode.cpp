#include "ptnode.h"
#include "dg.h"
#include "mgr.h"
//#define _PTNODE_NDEBUG_

extern Mgr* mgr;
PT*& pt = mgr->getPT();
static ofstream& logFile = mgr->getLogFile();
static PTNodeQueue& ptq = pt->getPTQ();
static Str2DGNodeMap& dgMap = pt->getDGMap();
static LCList& lcList = pt->getLCList();

//-------------base class-----------

void PTNode::setName(const string& name)
{
    _name = name;
}

void PTNode::writeDBG(const size_t& indent,size_t level) const
{
    logFile << string( indent * level ,' ') << _name << endl;
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->writeDBG(indent,level+1);
}

void PTNode::addChild(PTNode* n)
{
    _children.push_back(n);
}

const PTNodeList& PTNode::getChildren() const
{
    return _children;
}

//----------------writeDBG--------------
void PTVarIntNode::writeDBG(const size_t& indent , size_t level) const 
{
    logFile << string( indent * level ,' ')
             << _name << ((_isString)? " Y ":" N " )
             << "VAR_INT" << endl;
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->writeDBG(indent,level+1);
}

void PTVarBoolNode::writeDBG(const size_t& indent , size_t level) const 
{
    logFile << string( indent * level ,' ')
             << _name << ((_isString)? " Y ":" N " )
             << "VAR_BOOL" << endl;
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->writeDBG(indent,level+1);
}

void PTVarStringNode::writeDBG(const size_t& indent , size_t level) const 
{
    logFile << string( indent * level ,' ')
             << _name << ((_isString)? " Y ":" N " )
             << "VAR_STRING" << endl;
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->writeDBG(indent,level+1);
}

void PTConstIntNode::writeDBG(const size_t& indent , size_t level) const 
{
    logFile << string( indent * level ,' ')
             << _name << ((_isString)? " Y ":" N " )
             << "CONST_INT" << endl;
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->writeDBG(indent,level+1);
}

void PTConstBoolNode::writeDBG(const size_t& indent , size_t level) const 
{
    logFile << string( indent * level ,' ')
             << _name << ((_isString)? " Y ":" N " )
             << "CONST_BOOL" << endl;
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->writeDBG(indent,level+1);
}

void PTConstStringNode::writeDBG(const size_t& indent , size_t level) const 
{
    logFile << string( indent * level ,' ')
             << _name << ((_isString)? " Y ":" N " )
             << "CONST_STRING" << endl;
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->writeDBG(indent,level+1);
}

void PTIteNode::writeDBG(const size_t& indent , size_t level) const
{
    logFile << string( indent * level ,' ')
             << _name << ((_isString)? " Y ":" N " )
             << "ITE" << endl;
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->writeDBG(indent,level+1);
}

//-------------------buildDG------------------

DGNode* PTVarIntNode::buildDG()
{
    #ifndef _PTNODE_NDEBUG_
        logFile << _name << " VAR_INT" << endl;
    #endif
    return new DGNode(_name,VAR_INT);
}

DGNode* PTVarBoolNode::buildDG()
{
    #ifndef _PTNODE_NDEBUG_
        logFile << _name << " VAR_BOOL" << endl;
    #endif
    return 0;
}

DGNode* PTVarStringNode::buildDG()
{
    #ifndef _PTNODE_NDEBUG_
        logFile << _name << " VAR_STRING";
    #endif
    Str2DGNodeMap::iterator it = dgMap.find(_name);
    if (it != dgMap.end()) {
        #ifndef _PTNODE_NDEBUG_
            logFile << " => already exist , leader=" << it->second->findLeader()->getName() << " type=" << it->second->findLeader()->getType() << " isStringVar=" << it->second->findLeader()->isStringVar() << endl;
        #endif
        return it->second->findLeader();
    }
    else {
        DGNode* newNode = new DGNode(_name,VAR_STRING,1);
        dgMap.insert(Str2DGNode(_name,newNode));
#ifndef _PTNODE_NDEBUG_
logFile << " => create new node name=" << newNode->getName() << " isStringVar=" << newNode->isStringVar() << endl;
#endif
        return newNode;
    }
}

DGNode* PTConstIntNode::buildDG()
{
#ifndef _PTNODE_NDEBUG_
logFile << _name << " CONST_INT" << endl;
#endif
    //DGNode* n = new DGNode(_name,CONST_INT);
    //return n;
    return new DGNode(_name,CONST_INT);
}

DGNode* PTConstBoolNode::buildDG()
{
#ifndef _PTNODE_NDEBUG_
logFile << _name << " CONST_BOOL" << endl;
#endif
    return 0;
}

DGNode* PTConstStringNode::buildDG()
{
    string newName = pt->getNewNodeName();
#ifndef _PTNODE_NDEBUG_
logFile << _name << " CONST_STRING => create new node name=" << newName << endl;
#endif
    DGNode* newNode = new DGNode(newName,_name);
    dgMap.insert(Str2DGNode(newNode->getName(),newNode));
    return newNode;
}

//----------------bool type-------------------

DGNode* PTNotNode::buildDG()
{
    // irrelavent to automaton operation
#ifndef _PTNODE_NDEBUG_
logFile << _name << " => return 0" << endl;
#endif
    return 0;
}

DGNode* PTEqNode::buildDG()
{
    assert((_children.size() == 2));
    DGNode* left  = _children[0]->buildDG();
    DGNode* right = _children[1]->buildDG();
    
#ifndef _PTNODE_NDEBUG_
logFile << _name << " => left=";
if (!left) logFile << "0";
else {
    logFile << left->getName() << left->getTypeString() << endl;
}
logFile << " right=";
if (!right) logFile<< "0";
else {
    logFile << right->getName() << right->getTypeString() << endl;
}
#endif

    if (!left || !right) return 0;

    Type ltype = left->getType();
    Type rtype = right->getType();
    
    if (ltype == VAR_INT || ltype == CONST_INT || rtype == VAR_INT || rtype == CONST_INT || ltype>=40 && ltype<=50 || rtype>=40 && rtype<=50) {
        string lc = left->getName() + " == " + right->getName();
        lcList.push_back(lc);
        return 0;
    }
    else if (rtype == STRING_LEN) {
        assert((ltype == CONST_INT || ltype == VAR_INT || ltype>=40 && ltype<=50));
        string lc = left->getName() + " == " + right->getName();
        lcList.push_back(lc);
        return 0;
    }
    else if (ltype == STRING_LEN) {
        assert((rtype == CONST_INT || rtype == VAR_INT || rtype>=40 && rtype<=50));
        string lc = left->getName() + " == " + right->getName();
        lcList.push_back(lc);
        return 0;
    }
    else {
        string newLeftName  = pt->getNewNodeName();
        string newRightName = pt->getNewNodeName();
        #ifndef _PTNODE_NDEBUG_
            logFile << " newLeftName=" << newLeftName
                 << " newRightName=" << newRightName;
        #endif
        DGNode* newLeft  = new DGNode(newLeftName,left);
        DGNode* newRight = new DGNode(newRightName,right);
        //dgMap.insert(Str2DGNode(newLeftName,newLeft));
        //dgMap.insert(Str2DGNode(newRightName,newRight));
        left->clearChildren();
        right->clearChildren();
        if (left->isStringVar()) {
            #ifndef _PTNODE_NDEBUG_
                logFile << " set right=" << right->getName() << " leader=left" << endl;
            #endif
            right->setLeader(left);
            left->setType(AUT_INTER);
            left->addChild(newLeft);
            left->addChild(newRight);
            return left;
        }
        else if(right->isStringVar()) {
            #ifndef _PTNODE_NDEBUG_
                logFile << " set left=" << left->getName() << " leader=right" << endl;
            #endif
            left->setLeader(right);
            right->setType(AUT_INTER);
            right->addChild(newLeft);
            right->addChild(newRight);
            return right; 
        }
        else {
            logFile << " WARNING: [=] two operands are not VAR_STRING" << endl;
        }
    }
}

DGNode* PTNotEqNode::buildDG()
{
    assert((_children.size() == 2));
    DGNode* left  = _children[0]->buildDG();
    DGNode* right = _children[1]->buildDG();
    
#ifndef _PTNODE_NDEBUG_
logFile << _name << " => left=";
if (!left) logFile << "0";
else       logFile << left->getName();
logFile << " right=";
if (!right) logFile<< "0";
else        logFile<< right->getName();
#endif

    if (!left || !right) return 0;
    
    Type ltype = left->getType();
    Type rtype = right->getType();
    
    if (ltype == VAR_INT || ltype == CONST_INT || rtype == VAR_INT || rtype == CONST_INT || ltype>=40 && ltype<=50 || rtype>=40 && rtype<=50) {
#ifndef _PTNODE_NDEBUG_
logFile << " left = " << left->getName()
     << " right = " << right->getName();
#endif
        string lc = left->getName() + " != " + right->getName();
        lcList.push_back(lc);
        return 0;
    }
    else {
        assert((ltype == CONST_STRING || rtype == CONST_STRING));
        string newCompleName = pt->getNewNodeName();
        string newName = pt->getNewNodeName();
        DGNode* compleNode = new DGNode(newCompleName,AUT_COMPLE,0);
        compleNode->setNotSink();
        if (rtype == CONST_STRING) {
            #ifndef _PTNODE_NDEBUG_
                logFile << " set left=" << left->getName() << " to AUT_INTER" << endl;
            #endif
            left->clearChildren();
            DGNode* newNode = new DGNode(newName,left);
            compleNode->addChild(right);
            left->setType(AUT_INTER);
            left->addChild(newNode);
            left->addChild(compleNode);
            return left;
        }
        else {
            #ifndef _PTNODE_NDEBUG_
                logFile << " set right=" << right->getName() << " to AUT_INTER" << endl;
            #endif
            right->clearChildren();
            DGNode* newNode = new DGNode(newName,right);
            compleNode->addChild(left);
            right->setType(AUT_INTER);
            right->addChild(newNode);
            right->addChild(compleNode);
            return right;
        }
        /*
        string newName1 = pt->getNewNodeName();
        string newName2 = pt->getNewNodeName();
        string newName3 = pt->getNewNodeName();
        string newName4 = pt->getNewNodeName();
        string newCompleName1 = pt->getNewNodeName();
        string newCompleName2 = pt->getNewNodeName();
        #ifndef _PTNODE_NDEBUG_
        #endif
        DGNode* new1 = new DGNode(newName1,left);
        DGNode* new2 = new DGNode(newName2,right);
        DGNode* new3 = new DGNode(newName3,left);
        DGNode* new4 = new DGNode(newName4,right);
        DGNode* compleNode1 = new DGNode(newCompleName1,AUT_COMPLE,0);
        DGNode* compleNode2 = new DGNode(newCompleName2,AUT_COMPLE,0);

        compleNode1->addChild(new2);
        left->clearChildren();
        left->setType(AUT_INTER);
        left->addChild(new1);
        left->addChild(compleNode1);
    
        compleNode2->addChild(new4);
        right->clearChildren();
        right->setType(AUT_INTER);
        right->addChild(new3);
        right->addChild(compleNode2);
    
        return 0;
        */
    }
}

DGNode* PTAndNode::buildDG()
{
    #ifndef _PTNODE_NDEBUG_
        logFile << _name << " => return 0" << endl;
    #endif
    return 0;
}

DGNode* PTOrNode::buildDG()
{
    #ifndef _PTNODE_NDEBUG_
        logFile << _name << " => return 0" << endl;
    #endif
    return 0;
}

DGNode* PTIteNode::buildDG()
{
    assert((_children.size() == 3));
    #ifndef _PTNODE_NDEBUG_
        logFile << _name << " =>";
        logFile << " push " << _children[1]->getName();
        logFile << " push " << _children[2]->getName() << endl;
    #endif
    ptq.push(_children[1]);
    ptq.push(_children[2]);
    return 0;
}

DGNode* PTLTNode::buildDG()
{
    assert((_children.size() == 2));
    string lc = _children[0]->getName() + " " + _name + " " + _children[1]->getName();
    lcList.push_back(lc);
    return 0;
}

DGNode* PTLTOEQNode::buildDG()
{
    assert((_children.size() == 2));
    string lc = _children[0]->getName() + " " + _name + " " + _children[1]->getName();
    lcList.push_back(lc);
    return 0;
}

DGNode* PTMTNode::buildDG()
{
    assert((_children.size() == 2));
    string lc = _children[0]->getName() + " " + _name + " " + _children[1]->getName();
    lcList.push_back(lc);
    return 0;
}

DGNode* PTMTOEQNode::buildDG()
{
    assert((_children.size() == 2));
    string lc = _children[0]->getName() + " " + _name + " " + _children[1]->getName();
    lcList.push_back(lc);
    return 0;
}

//-----------------int type-------------------

DGNode* PTPlusNode::buildDG()
{
    size_t s = _children.size();
    if (s == 1) {
#ifndef _PTNODE_NDEBUG_
logFile << _name << "INT_POS" << endl;
#endif
        DGNode* n = _children[0]->buildDG();
        return new DGNode(n->getName(),INT_POS);
    }
    else {
        assert((s == 2));
#ifndef _PTNODE_NDEBUG_
logFile << _name << "INT_PLUS" << endl;
#endif
        DGNode* left  = _children[0]->buildDG();
        DGNode* right = _children[1]->buildDG();

        string s = left->getName() + " " + _name + " " + right->getName();
        return new DGNode(s,INT_PLUS);
    }
}

DGNode* PTMinusNode::buildDG()
{
    size_t s = _children.size();
    if (s == 1) {
#ifndef _PTNODE_NDEBUG_
logFile << _name << "INT_NEG" << endl;
#endif
        DGNode* n = _children[0]->buildDG();
        logFile << n->getName();
        string s = _name + n->getName();
#ifndef _PTNODE_NDEBUG_
logFile << " => return -" << n->getName();
#endif
        return new DGNode(s,INT_NEG);
    }
    else {
        assert((s == 2));
#ifndef _PTNODE_NDEBUG_
logFile << _name << "INT_MINUS" << endl;
#endif
        DGNode* left  = _children[0]->buildDG();
        DGNode* right = _children[1]->buildDG();

        string s = left->getName() + " " + _name + " " + right->getName();
        return new DGNode(s,INT_MINUS);
    }
}

DGNode* PTDivNode::buildDG()
{
    assert((_children.size() == 2));
#ifndef _PTNODE_NDEBUG_
logFile << _name << "INT_DIV" << endl;
#endif
    DGNode* left  = _children[0]->buildDG();
    DGNode* right = _children[1]->buildDG();

    string s = left->getName() + " / " + right->getName();
    return new DGNode(s,INT_DIV);
}

//-----------------string type-------------------

DGNode* PTStrConcateNode::buildDG()
{
    string newName = pt->getNewNodeName();
    #ifndef _PTNODE_NDEBUG_
        logFile << _name << " => create new node name=" << newName << endl;
    #endif
    DGNode* newNode = new DGNode(newName,AUT_CONCATE,0);
    dgMap.insert(Str2DGNode(newName,newNode));
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
    DGNode* cur = _children[0]->buildDG();
    cur->setLength();
    #ifndef _PTNODE_NDEBUG_
        logFile << _name << " => child=" << cur->getName() << " set length" << endl;
    #endif
    return new DGNode(_children[0]->getName(),STRING_LEN);
}

DGNode* PTStrInReNode::buildDG()
{
    assert((_children.size() == 2));
    DGNode* left  = _children[0]->buildDG();
    DGNode* right = _children[1]->buildDG();
    
    assert((left && right));
    string newLeftName = pt->getNewNodeName();
    #ifndef _PTNODE_NDEBUG_
        //logFile << _name << " => left=" << (left? left->getName():"0")
        //              << " right=" << (right? right->getName():"0");
        logFile << _name << " => left=" <<  left->getName()
                      << " right=" << right->getName()
                      << " newLeftName=" << newLeftName << endl;
    #endif
    DGNode* newNode = new DGNode(newLeftName,left);
    //dgMap.insert(Str2DGNode(newLeftName,newNode));
    right->setNotSink();
    left->setType(AUT_INTER);
    left->clearChildren();
    left->addChild(newNode);
    left->addChild(right);
    
    return left;
    /*
    //TODO: need to consider symmetry
    if (left->getType() == VAR_STRING) {
        if (right->getType() == CONST_STRING) {
            #ifndef _PTNODE_NDEBUG_
                logFile << " => left VSCS right ; erase right" << endl;
            #endif
            left->mergeVSCS(right);
        }
        else {
            #ifndef _PTNODE_NDEBUG_
                logFile << " => left VSOS right ; erase right" << endl;
            #endif
            left->mergeVSOS(right);
        }
        dgMap.erase(dgMap.find(right->getName()));
        delete right;
        return left;
    }
    else {
        logFile << _name << endl;
        assert((left->getType() == OP_STRCONCATE) || (left->getType() == OP_STRREPLACE));
        string newName = pt->getNewNodeName();
        DGNode* newNode = new DGNode(newName,OP_REINTER);
        dgMap.insert(Str2DGNode(newName,newNode));
        left->setNotSink();
        right->setNotSink();
        newNode->addChild(left);
        newNode->addChild(right);
        #ifndef _PTNODE_NDEBUG_
            logFile << " => create intersect name=" << newNode->getName() << endl;
        #endif
        return newNode;
    }
    */
}

DGNode* PTStrNotInReNode::buildDG()
{
    assert((_children.size() == 2));
    DGNode* left  = _children[0]->buildDG();
    DGNode* right = _children[1]->buildDG();
    
    assert((left && right));
    string newCompleName = pt->getNewNodeName();
    string newLeftName   = pt->getNewNodeName();
    #ifndef _PTNODE_NDEBUG_
        //logFile << _name << " => left=" << (left? left->getName():"0")
        //              << " right=" << (right? right->getName():"0");
        logFile << _name << " => left=" <<  left->getName()
                      << " right=" << right->getName() 
                      << " newLeftName=" << newLeftName
                      << " comple=" << newCompleName << endl;
    #endif
    DGNode* newLeftNode = new DGNode(newLeftName,left);
    DGNode* compleNode = new DGNode(newCompleName,AUT_COMPLE,0);
    //dgMap.insert(Str2DGNode(newLeftName,newLeftNode));
    //dgMap.insert(Str2DGNode(newCompleName,compleNode));
    right->setNotSink();
    compleNode->setNotSink();
    compleNode->addChild(right);
    left->setType(AUT_INTER);
    left->clearChildren();
    left->addChild(newLeftNode);
    left->addChild(compleNode);
    return left;
}

DGNode* PTStrReplaceNode::buildDG()
{
    string newName = pt->getNewNodeName();
    #ifndef _PTNODE_NDEBUG_
        logFile << _name << " => create new node name=" << newName << endl;
    #endif
    assert((_children.size() == 3));
    DGNode* newNode = new DGNode(newName,AUT_REPLACE,0);
    dgMap.insert(Str2DGNode(newName,newNode));
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
    #ifndef _DEBUG_
        logFile << _name << " => return child=" << _children[0]->getName() << endl;
    #endif
    return _children[0]->buildDG();
}

//-----------------automaton type-------------------

DGNode* PTReConcateNode::buildDG()
{
    string newName = pt->getNewNodeName();
    #ifndef _PTNODE_NDEBUG_
        logFile << _name << " => create new node name=" << newName << endl;
    #endif
    DGNode* newNode = new DGNode(newName,AUT_CONCATE,0);
    dgMap.insert(Str2DGNode(newName,newNode));
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
    string newName = pt->getNewNodeName();
    #ifndef _PTNODE_NDEBUG_
        logFile << _name << " => create new node name=" << newName << endl;
    #endif
    DGNode* newNode = new DGNode(newName,AUT_UNION,0);
    dgMap.insert(Str2DGNode(newName,newNode));
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
    string newName = pt->getNewNodeName();
    #ifndef _PTNODE_NDEBUG_
        logFile << _name << " => create new node name=" << newName << endl;
    #endif
    DGNode* newNode = new DGNode(newName,AUT_INTER,0);
    dgMap.insert(Str2DGNode(newName,newNode));
    for (PTNodeList::iterator it=_children.begin(); it!=_children.end(); ++it) {
        DGNode* cur = (*it)->buildDG();
        assert(cur);
        cur->setNotSink();
        newNode->addChild(cur);
    }
    return newNode;
}

