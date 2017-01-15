#include "ptnode.h"
#include "dg.h"
#include "mgr.h"
#define _PTNODE_NDEBUG_

extern Mgr* mgr;

//-------------base class-----------

void PTNode::setName(const string& name)
{
    _name = name;
}

void PTNode::print(const size_t& indent,size_t level) const
{
    cout << string( indent * level ,' ') << _name << endl;
         //<< _name << ((_isString)? " Y":" N" ) << endl;
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->print(indent,level+1);
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
    cout << string( indent * level ,' ')
         << _name << ((_isString)? " Y ":" N " )
         << "VAR_INT" << endl;
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->print(indent,level+1);
}

void PTVarBoolNode::print(const size_t& indent , size_t level) const 
{
    cout << string( indent * level ,' ')
         << _name << ((_isString)? " Y ":" N " )
         << "VAR_BOOL" << endl;
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->print(indent,level+1);
}

void PTVarStringNode::print(const size_t& indent , size_t level) const 
{
    cout << string( indent * level ,' ')
         << _name << ((_isString)? " Y ":" N " )
         << "VAR_STRING" << endl;
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->print(indent,level+1);
}

void PTConstIntNode::print(const size_t& indent , size_t level) const 
{
    cout << string( indent * level ,' ')
         << _name << ((_isString)? " Y ":" N " )
         << "CONST_INT" << endl;
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->print(indent,level+1);
}

void PTConstBoolNode::print(const size_t& indent , size_t level) const 
{
    cout << string( indent * level ,' ')
         << _name << ((_isString)? " Y ":" N " )
         << "CONST_BOOL" << endl;
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->print(indent,level+1);
}

void PTConstStringNode::print(const size_t& indent , size_t level) const 
{
    cout << string( indent * level ,' ')
         << _name << ((_isString)? " Y ":" N " )
         << "CONST_STRING" << endl;
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->print(indent,level+1);
}

void PTIteNode::print(const size_t& indent , size_t level) const
{
    cout << string( indent * level ,' ')
         << _name << ((_isString)? " Y ":" N " )
         << "ITE" << endl;
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->print(indent,level+1);
}

//-------------------buildDG------------------

DGNode* PTVarIntNode::buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap )
{
    #ifndef _PTNODE_NDEBUG_
        cout << _name << " VAR_INT" << endl;
    #endif
    return 0;
}

DGNode* PTVarBoolNode::buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap )
{
    #ifndef _PTNODE_NDEBUG_
        cout << _name << " VAR_BOOL" << endl;
    #endif
    return 0;
}

DGNode* PTVarStringNode::buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap )
{
    #ifndef _PTNODE_NDEBUG_
        cout << _name << " VAR_STRING";
    #endif
    Str2DGNodeMap::iterator it = dgMap.find(_name);
    if (it != dgMap.end()) {
        #ifndef _PTNODE_NDEBUG_
            cout << " => already exist , leader=" << it->second->findLeader()->getName() << " type=" << it->second->findLeader()->getType() << " isStringVar=" << it->second->findLeader()->isStringVar() << endl;
        #endif
        return it->second->findLeader();
    }
    else {
        DGNode* newNode = new DGNode(_name,VAR_STRING,1);
        dgMap.insert(Str2DGNode(_name,newNode));
        #ifndef _PTNODE_NDEBUG_
            cout << " => create new node name=" << newNode->getName() << " isStringVar=" << newNode->isStringVar() << endl;
        #endif
        return newNode;
    }
}

DGNode* PTConstIntNode::buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap )
{
    #ifndef _PTNODE_NDEBUG_
        cout << _name << " CONST_INT" << endl;
    #endif
    return 0;
}

DGNode* PTConstBoolNode::buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap )
{
    #ifndef _PTNODE_NDEBUG_
        cout << _name << " CONST_BOOL" << endl;
    #endif
    return 0;
}

DGNode* PTConstStringNode::buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap )
{
    string newName = mgr->getNewNodeName();
    #ifndef _PTNODE_NDEBUG_
        cout << _name << " CONST_STRING => create new node name=" << newName << endl;
    #endif
    DGNode* newNode = new DGNode(newName,_name);
    dgMap.insert(Str2DGNode(newNode->getName(),newNode));
    return newNode;
}

DGNode* PTNotNode::buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap )
{
    // irrelavent to automaton operation
    #ifndef _PTNODE_NDEBUG_
        cout << _name << " => return 0" << endl;
    #endif
    return 0;
}

DGNode* PTPlusNode::buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap )
{
    // irrelavent to automaton operation
    #ifndef _PTNODE_NDEBUG_
        cout << _name << " => return 0" << endl;
    #endif
    return 0;
}

DGNode* PTNegNode::buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap )
{
    // irrelavent to automaton operation
    #ifndef _PTNODE_NDEBUG_
        cout << _name << " => return 0" << endl;
    #endif
    return 0;
}

DGNode* PTEqNode::buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap )
{
    assert((_children.size() == 2));
    DGNode* left  = _children[0]->buildDG(ptq,dgMap);
    DGNode* right = _children[1]->buildDG(ptq,dgMap);
    
    #ifndef _PTNODE_NDEBUG_
        cout << _name << " => left=";
        if (!left) cout << "0";
        else       cout << left->getName();
        cout << " right=";
        if (!right) cout<< "0";
        else        cout<< right->getName();
    #endif

    if (!left || !right) return 0;
    string newLeftName  = mgr->getNewNodeName();
    string newRightName = mgr->getNewNodeName();
    #ifndef _PTNODE_NDEBUG_
        cout << " newLeftName=" << newLeftName
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
            cout << " set right=" << right->getName() << " leader=left" << endl;
        #endif
        right->setLeader(left);
        left->setType(AUT_INTER);
        left->addChild(newLeft);
        left->addChild(newRight);
        return left;
    }
    else if(right->isStringVar()) {
        #ifndef _PTNODE_NDEBUG_
            cout << " set left=" << left->getName() << " leader=right" << endl;
        #endif
        left->setLeader(right);
        right->setType(AUT_INTER);
        right->addChild(newLeft);
        right->addChild(newRight);
        return right; 
    }
    else {
        cout << " [=] 2 operand are not VAR_STRING" << endl;
    }
    /*
    //TODO: need to consider symmerty
    assert((left->getType() == ));
    if (left->getType() == VAR_STRING){
        const Type& rtype = right->getType();
        if (rtype == OP_CONCATE || rtype == OP_REPLACE) {
            #ifndef _PTNODE_NDEBUG_
                cout << "  => left VSOS right ; erase right" << endl;
            #endif
            left->mergeVSOS(right);
            dgMap.erase(dgMap.find(right->getName()));
            delete right;
            return left;
        }
        else if (rtype == CONST_STRING) {
            #ifndef _PTNODE_NDEBUG_
                cout << "  => left VSCS right ; erase right" << endl;
            #endif
            left->mergeVSCS(right);
            dgMap.erase(dgMap.find(right->getName()));
            delete right;
            return left;
        }
        else if (rtype == VAR_STRING) {
            #ifndef _PTNODE_NDEBUG_
                cout << "  => left=right=VAR_STRING => return 0" << endl;
            #endif
            return 0;
        }
    }
    else if(left->getType() == )
    */
}

DGNode* PTAndNode::buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap )
{
    #ifndef _PTNODE_NDEBUG_
        cout << _name << " => return 0" << endl;
    #endif
    return 0;
}

DGNode* PTOrNode::buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap )
{
    #ifndef _PTNODE_NDEBUG_
        cout << _name << " => return 0" << endl;
    #endif
    return 0;
}

DGNode* PTIteNode::buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap )
{
    assert((_children.size() == 3));
    #ifndef _PTNODE_NDEBUG_
        cout << _name << " =>";
        cout << " push " << _children[1]->getName();
        cout << " push " << _children[2]->getName() << endl;
    #endif
    ptq.push(_children[1]);
    ptq.push(_children[2]);
    return 0;
}

DGNode* PTStrConcateNode::buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap )
{
    string newName = mgr->getNewNodeName();
    #ifndef _PTNODE_NDEBUG_
        cout << _name << " => create new node name=" << newName << endl;
    #endif
    DGNode* newNode = new DGNode(newName,AUT_CONCATE,0);
    dgMap.insert(Str2DGNode(newName,newNode));
    for (PTNodeList::iterator it=_children.begin(); it!=_children.end(); ++it) {
        DGNode* cur = (*it)->buildDG(ptq,dgMap);
        assert(cur);
        cur->setNotSink();
        newNode->addChild(cur);
    }
    return newNode;
}

DGNode* PTStrLenNode::buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap )
{
    assert((_children.size() == 1));
    DGNode* cur = _children[0]->buildDG(ptq,dgMap);
    cur->setLength();
    #ifndef _PTNODE_NDEBUG_
        cout << _name << " => child=" << cur->getName() << " set length" << endl;
    #endif
    return cur;
}

DGNode* PTStrInReNode::buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap )
{
    assert((_children.size() == 2));
    DGNode* left  = _children[0]->buildDG(ptq,dgMap);
    DGNode* right = _children[1]->buildDG(ptq,dgMap);
    
    assert((left && right));
    string newLeftName = mgr->getNewNodeName();
    #ifndef _PTNODE_NDEBUG_
        //cout << _name << " => left=" << (left? left->getName():"0")
        //              << " right=" << (right? right->getName():"0");
        cout << _name << " => left=" <<  left->getName()
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
                cout << " => left VSCS right ; erase right" << endl;
            #endif
            left->mergeVSCS(right);
        }
        else {
            #ifndef _PTNODE_NDEBUG_
                cout << " => left VSOS right ; erase right" << endl;
            #endif
            left->mergeVSOS(right);
        }
        dgMap.erase(dgMap.find(right->getName()));
        delete right;
        return left;
    }
    else {
        cout << _name << endl;
        assert((left->getType() == OP_STRCONCATE) || (left->getType() == OP_STRREPLACE));
        string newName = mgr->getNewNodeName();
        DGNode* newNode = new DGNode(newName,OP_REINTER);
        dgMap.insert(Str2DGNode(newName,newNode));
        left->setNotSink();
        right->setNotSink();
        newNode->addChild(left);
        newNode->addChild(right);
        #ifndef _PTNODE_NDEBUG_
            cout << " => create intersect name=" << newNode->getName() << endl;
        #endif
        return newNode;
    }
    */
}

DGNode* PTStrNotInReNode::buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap )
{
    assert((_children.size() == 2));
    DGNode* left  = _children[0]->buildDG(ptq,dgMap);
    DGNode* right = _children[1]->buildDG(ptq,dgMap);
    
    assert((left && right));
    string newCompleName = mgr->getNewNodeName();
    string newLeftName   = mgr->getNewNodeName();
    #ifndef _PTNODE_NDEBUG_
        //cout << _name << " => left=" << (left? left->getName():"0")
        //              << " right=" << (right? right->getName():"0");
        cout << _name << " => left=" <<  left->getName()
                      << " right=" << right->getName() 
                      << " newLeftName=" << newLeftName
                      << " comple=" << newCompleName << endl;
    #endif
    DGNode* newLeftNode = new DGNode(newLeftName,left);
    DGNode* compleNode = new DGNode(newCompleName,AUT_COMPLE,0);
    //dgMap.insert(Str2DGNode(newLeftName,newLeftNode));
    //dgMap.insert(Str2DGNode(newCompleName,compleNode));
    right->setNotSink();
    compleNode->addChild(right);
    left->setType(AUT_INTER);
    left->clearChildren();
    left->addChild(newLeftNode);
    left->addChild(compleNode);
    return left;
}

DGNode* PTStrReplaceNode::buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap )
{
    string newName = mgr->getNewNodeName();
    #ifndef _PTNODE_NDEBUG_
        cout << _name << " => create new node name=" << newName << endl;
    #endif
    assert((_children.size() == 3));
    DGNode* newNode = new DGNode(newName,AUT_REPLACE,0);
    dgMap.insert(Str2DGNode(newName,newNode));
    for (PTNodeList::iterator it=_children.begin(); it!=_children.end(); ++it) {
        DGNode* cur = (*it)->buildDG(ptq,dgMap);
        assert(cur);
        cur->setNotSink();
        newNode->addChild(cur);
    }
    return newNode;
}

DGNode* PTStrToReNode::buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap )
{
    assert((_children.size() == 1));
    #ifndef _DEBUG_
        cout << _name << " => return child=" << _children[0]->getName() << endl;
    #endif
    return _children[0]->buildDG(ptq,dgMap);
}

DGNode* PTReConcateNode::buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap )
{
    string newName = mgr->getNewNodeName();
    #ifndef _PTNODE_NDEBUG_
        cout << _name << " => create new node name=" << newName << endl;
    #endif
    DGNode* newNode = new DGNode(newName,AUT_CONCATE,0);
    dgMap.insert(Str2DGNode(newName,newNode));
    for (PTNodeList::iterator it=_children.begin(); it!=_children.end(); ++it) {
        DGNode* cur = (*it)->buildDG(ptq,dgMap);
        assert(cur);
        cur->setNotSink();
        newNode->addChild(cur);
    }
    return newNode;
}

DGNode* PTReUnionNode::buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap )
{
    string newName = mgr->getNewNodeName();
    #ifndef _PTNODE_NDEBUG_
        cout << _name << " => create new node name=" << newName << endl;
    #endif
    DGNode* newNode = new DGNode(newName,AUT_UNION,0);
    dgMap.insert(Str2DGNode(newName,newNode));
    for (PTNodeList::iterator it=_children.begin(); it!=_children.end(); ++it) {
        DGNode* cur = (*it)->buildDG(ptq,dgMap);
        assert(cur);
        cur->setNotSink();
        newNode->addChild(cur);
    }
    return newNode;
}

DGNode* PTReInterNode::buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap )
{
    string newName = mgr->getNewNodeName();
    #ifndef _PTNODE_NDEBUG_
        cout << _name << " => create new node name=" << newName << endl;
    #endif
    DGNode* newNode = new DGNode(newName,AUT_INTER,0);
    dgMap.insert(Str2DGNode(newName,newNode));
    for (PTNodeList::iterator it=_children.begin(); it!=_children.end(); ++it) {
        DGNode* cur = (*it)->buildDG(ptq,dgMap);
        assert(cur);
        cur->setNotSink();
        newNode->addChild(cur);
    }
    return newNode;
}

