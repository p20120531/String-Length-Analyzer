#include "pt.h"

void PT::addAssertion(PTNode* n)
{
    _root->addChild(n);
}

void PT::print(const size_t& indent) const
{
    for (size_t i = 0, size = _root->_children.size(); i < size; ++i) {
        cout << _root->_name << " " << i + 1 << endl;
        _root->_children[i]->print(indent,0);
    }
}

void PT::mergeNotAndStrInRe()
{
    for (size_t i = 0, size = _root->_children.size(); i < size; ++i) {
        PTNodeQueue ptq;
        ptq.push(_root->_children[i]);
        while (!ptq.empty()) {
            PTNode* p = ptq.front();
            ptq.pop();
            for (PTNodeList::iterator it = p->_children.begin(); it != p->_children.end(); ++it) {
                if ((*it)->_name == "not") {
                    assert(((*it)->_children.size() == 1));
                    PTNode*& nChild = (*it)->_children[0];
                    if (nChild->_name == "str.in.re") {
                        PTNode* newNode = new PTStrNotInReNode("str.notin.re");
                        for (size_t j=0,size=nChild->_children.size();j<size;++j) {
                            newNode->addChild(nChild->_children[j]);
                        }
                        p->_children.erase(it);
                        p->addChild(newNode);
                        ptq.push(newNode);
                    }
                    else
                        ptq.push(nChild);
                }
                else 
                    ptq.push(*it);
            }
        }
    }
}

