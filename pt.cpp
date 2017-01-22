#include "pt.h"
#include "mgr.h"
//#define _PT_NDBG_
extern Mgr* mgr;
static ofstream& logFile = mgr->getLogFile();

void PT::addAssertion(PTNode* n)
{
    _root->addChild(n);
}

void PT::writeDBG() const
{
    #ifndef _PT_NDBG_
        splitLine(logFile,"PT::writeDBG()");
    #endif
    for (size_t i = 0, size = _root->_children.size(); i < size; ++i) {
        #ifndef _PT_NDBG_
            logFile << _root->_name << " " << i + 1 << endl;
        #endif
        _root->_children[i]->writeDBG(_indent,0);
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
                else if ((*it)->_children[0]->_name == "=") {
                    PTNode* newNode = new PTNotEqNode("!=");
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
    return string("NEW_DGNode_") + itos(++_newDGNodeCnt);
}
