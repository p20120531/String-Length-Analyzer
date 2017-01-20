#include "pt.h"
#define _PT_MERGENDBG_
void PT::addAssertion(PTNode* n)
{
    _root->addChild(n);
}

void PT::print() const
{
    for (size_t i = 0, size = _root->_children.size(); i < size; ++i) {
        cout << _root->_name << " " << i + 1 << endl;
        _root->_children[i]->print(_indent,0);
    }
}

void PT::mergeNotAndStrInRe()
{
    PTNodeQueue ptq;
    ptq.push(_root);
    while (!ptq.empty()) {
        PTNode* p = ptq.front();
        ptq.pop();
        for (PTNodeList::iterator it = p->_children.begin(); it != p->_children.end(); ++it) {
            if ((*it)->_name == "not") {
                assert(((*it)->_children.size() == 1));
                if ((*it)->_children[0]->_name == "str.in.re") {
                    PTNode* newNode = new PTStrNotInReNode("str.notin.re");
                    newNode->_children = (*it)->_children[0]->_children;
                    p->_children.erase(it);
                    --it;
                    p->addChild(newNode);
                    ptq.push(newNode);
                }
                else
                    ptq.push(*it);
            }
            else 
                ptq.push(*it);
        }
    }
}

string PT::getNewNodeName()
{
    stringstream ss;
    ss << ++_newDGNodeCnt;
    return string("NEW_DGNode_") + ss.str();
}
