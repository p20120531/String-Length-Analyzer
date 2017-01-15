#include "mgr.h"

void Mgr::printDG() const
{
    cout << "dgList = " << _dgList.size() << endl;
    for (size_t i = 0,size = _dgList.size(); i < size; ++i) {
        cout << "DG " << i + 1 << endl;
        
        cout << "printDBG" << endl;
        _dgList[i]->printDBG();
        cout << "merge" << endl;
        _dgList[i]->merge();
        cout << "printDBG" << endl;
        _dgList[i]->printDBG();
        cout << "print" << endl;
        _dgList[i]->print();
        cout << endl;
        
        /*
        _dgList[i]->merge();
        _dgList[i]->print();
        cout << endl;
        */
    }
}

string Mgr::getNewNodeName()
{
    stringstream ss;
    ss << ++_newDGNodeCnt;
    return string("NEW_DGNode_") + ss.str();
}

void Mgr::buildDG()
{
    PTNodeQueue ptq;
    ptq.push(_pt->_root);
    while (!ptq.empty()) {
        Str2DGNodeMap dgMap;
        buildDG_r(ptq,dgMap);
    }

}

void Mgr::buildDG_r(PTNodeQueue& ptq , Str2DGNodeMap& dgMap)
{
    PTNode* cur = ptq.front();
    ptq.pop();
    cout << "handle " << cur->getName() << endl;
    for (size_t i = 0, size = cur->_children.size(); i < size ; ++i) {
        DGNode* newNode = cur->_children[i]->buildDG(ptq,dgMap);
    }
    for (Str2DGNodeMap::iterator it=dgMap.begin(); it!=dgMap.end(); ++it) {
        DGNode* leader = it->second->findLeader();
        if (leader->isSink() && leader->getFlag() != _dgFlag) {
            cout << "found sink node = " << leader->getName() << endl;
            leader->setFlag(_dgFlag);
            _dgList.push_back(new DG(leader));
        }
    }
}        
