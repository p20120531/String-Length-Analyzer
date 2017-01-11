#include "ptnode.h"
#include "dg.h"
#include "mgr.h"
//#define _NDEBUG_

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
    #ifndef _NDEBUG_
        cout << _name << " VAR_INT" << endl;
    #endif
    return 0;
}

DGNode* PTVarBoolNode::buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap )
{
    #ifndef _NDEBUG_
        cout << _name << " VAR_BOOL" << endl;
    #endif
    return 0;
}

DGNode* PTVarStringNode::buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap )
{
    #ifndef _NDEBUG_
        cout << _name << " VAR_STRING";
    #endif
    Str2DGNodeMap::iterator it = dgMap.find(_name);
    if (it != dgMap.end()) {
        #ifndef _NDEBUG_
            cout << " => already exist" << endl;
        #endif
        return it->second;
    }
    else {
        DGNode* newNode = new DGNode(_name,VAR_STRING);
        dgMap.insert(Str2DGNode(_name,newNode));
        #ifndef _NDEBUG_
            cout << " => create new node" << endl;
        #endif
        return newNode;
    }
}

DGNode* PTConstIntNode::buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap )
{
    #ifndef _NDEBUG_
        cout << _name << " CONST_INT" << endl;
    #endif
    return 0;
}

DGNode* PTConstBoolNode::buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap )
{
    #ifndef _NDEBUG_
        cout << _name << " CONST_BOOL" << endl;
    #endif
    return 0;
}

DGNode* PTConstStringNode::buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap )
{
    string newName = mgr->getNewNodeName(CONST_STRING);
    #ifndef _NDEBUG_
        cout << _name << " CONST_STRING => create new node name=" << newName << endl;
    #endif
    DGNode* newNode = new DGNode(newName,_name);
    dgMap.insert(Str2DGNode(newNode->getName(),newNode));
    return newNode;
}

DGNode* PTNotNode::buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap )
{
    // currently skipped
    assert((_children.size() == 1));
    DGNode* nxt = _children[0]->buildDG(ptq,dgMap);
    #ifndef _NDEBUG_
        if (nxt)
            cout << _name << " => nxt=" << nxt->getName() << endl;
    #endif
    return nxt;
}

DGNode* PTPlusNode::buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap )
{
    // irrelavent to automoton operation
    #ifndef _NDEBUG_
        cout << _name << " => return 0" << endl;
    #endif
    return 0;
}

DGNode* PTNegNode::buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap )
{
    // irrelavent to automoton operation
    #ifndef _NDEBUG_
        cout << _name << " => return 0" << endl;
    #endif
    return 0;
}

DGNode* PTEqNode::buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap )
{
    assert((_children.size() == 2));
    DGNode* left  = _children[0]->buildDG(ptq,dgMap);
    DGNode* right = _children[1]->buildDG(ptq,dgMap);
    
    #ifndef _NDEBUG_
        cout << _name << " => left=";
        if (!left) cout << "0";
        else       cout << left->getName();
        cout << " right=";
        if (!right) cout<< "0";
        else        cout<< right->getName();
        cout << endl;
    #endif

    if (!left || !right) return 0;
    //TODO: need to consider symmerty
    if (left->getType() == VAR_STRING){
        const Type& rtype = right->getType();
        if (rtype == OP_STRCONCATE || rtype == OP_STRREPLACE) {
            #ifndef _NDEBUG_
                cout << "  => left VSOS right ; erase right" << endl;
            #endif
            left->mergeVSOS(right);
            dgMap.erase(dgMap.find(right->getName()));
            delete right;
            return left;
        }
        else if (rtype == CONST_STRING) {
            #ifndef _NDEBUG_
                cout << "  => left VSCS right ; erase right" << endl;
            #endif
            left->mergeVSCS(right);
            dgMap.erase(dgMap.find(right->getName()));
            delete right;
            return left;
        }
        else if (rtype == VAR_STRING) {
            #ifndef _NDEBUG_
                cout << "  => left=right=VAR_STRING => return 0" << endl;
            #endif
            return 0;
        }
    }
}

DGNode* PTAndNode::buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap )
{
    #ifndef _NDEBUG_
        cout << _name << " => return 0" << endl;
    #endif
    return 0;
}

DGNode* PTOrNode::buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap )
{
    #ifndef _NDEBUG_
        cout << _name << " => return 0" << endl;
    #endif
    return 0;
}

DGNode* PTIteNode::buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap )
{
    assert((_children.size() == 3));
    #ifndef _NDEBUG_
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
    string newName = mgr->getNewNodeName(OP_STRCONCATE);
    #ifndef _NDEBUG_
        cout << _name << " => create new node name=" << newName << endl;
    #endif
    DGNode* newNode = new DGNode(newName,OP_STRCONCATE);
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
    #ifndef _NDEBUG_
        cout << _name << " => child=" << cur->getName() << " set length" << endl;
    #endif
    return cur;
}

DGNode* PTStrInReNode::buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap )
{
    assert((_children.size() == 2));
    DGNode* left  = _children[0]->buildDG(ptq,dgMap);
    DGNode* right = _children[1]->buildDG(ptq,dgMap);
    
    #ifndef _NDEBUG_
        cout << _name << " => left=" << (left? left->getName():"0")
                      << " right=" << (right? right->getName():"0");
    #endif
    //TODO: need to consider symmetry
    if (left->getType() == VAR_STRING) {
        if (right->getType() == CONST_STRING) {
            #ifndef _NDEBUG_
                cout << " => left VSCS right ; erase right" << endl;
            #endif
            left->mergeVSCS(right);
        }
        else {
            #ifndef _NDEBUG_
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
        string newName = mgr->getNewNodeName(OP_REINTER);
        DGNode* newNode = new DGNode(newName,OP_REINTER);
        dgMap.insert(Str2DGNode(newName,newNode));
        left->setNotSink();
        right->setNotSink();
        newNode->addChild(left);
        newNode->addChild(right);
        #ifndef _NDEBUG_
            cout << " => create intersect name=" << newNode->getName() << endl;
        #endif
        return newNode;
    }
}

DGNode* PTStrReplaceNode::buildDG( PTNodeQueue& ptq , Str2DGNodeMap& dgMap )
{
    string newName = mgr->getNewNodeName(OP_STRREPLACE);
    #ifndef _NDEBUG_
        cout << _name << " => create new node name=" << newName << endl;
    #endif
    assert((_children.size() == 3));
    DGNode* newNode = new DGNode(newName,OP_STRREPLACE);
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
    string newName = mgr->getNewNodeName(OP_RECONCATE);
    #ifndef _NDEBUG_
        cout << _name << " => create new node name=" << newName << endl;
    #endif
    DGNode* newNode = new DGNode(newName,OP_RECONCATE);
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
    string newName = mgr->getNewNodeName(OP_REUNION);
    #ifndef _NDEBUG_
        cout << _name << " => create new node name=" << newName << endl;
    #endif
    DGNode* newNode = new DGNode(newName,OP_REUNION);
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
    string newName = mgr->getNewNodeName(OP_REINTER);
    #ifndef _NDEBUG_
        cout << _name << " => create new node name=" << newName << endl;
    #endif
    DGNode* newNode = new DGNode(newName,OP_REINTER);
    dgMap.insert(Str2DGNode(newName,newNode));
    for (PTNodeList::iterator it=_children.begin(); it!=_children.end(); ++it) {
        DGNode* cur = (*it)->buildDG(ptq,dgMap);
        assert(cur);
        cur->setNotSink();
        newNode->addChild(cur);
    }
    return newNode;
}

