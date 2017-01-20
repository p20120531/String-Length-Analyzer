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
        _dgList[i]->writeCmdFile();
        cout << endl; 
    }
}

void Mgr::buildDG()
{
    ++_gflag;
    size_t dgIdx = 0;
    PTNodeQueue&   ptq   = _pt->getPTQ();
    Str2DGNodeMap& dgMap = _pt->getDGMap();
    ptq.push(_pt->_root);
    while (!ptq.empty()) {
        (_pt->getDGMap()).clear();
        PTNode* cur = ptq.front();
        ptq.pop();
        cout << "handle " << cur->getName() << endl;
        for (PTNodeList::iterator it=cur->_children.begin();it!=cur->_children.end();++it)
            DGNode* newNode = (*it)->buildDG();
        for (Str2DGNodeMap::iterator it=dgMap.begin(); it!=dgMap.end(); ++it) {
            DGNode* leader = it->second->findLeader();
            if (leader->isSink() && leader->getFlag() != _gflag) {
                cout << "found sink node = " << leader->getName() << endl;
                leader->setFlag(_gflag);
                _dgList.push_back(new DG(leader,_indent,_gflag,_path,++dgIdx));
            }
        }
    }

}
