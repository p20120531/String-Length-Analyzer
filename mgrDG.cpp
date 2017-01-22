#include "mgr.h"

void Mgr::printDG()
{
    splitLine(_logFile,"Mgr::printDG()");
    _logFile << "dgList = " << _dgList.size() << endl;
    for (size_t i = 0,size = _dgList.size(); i < size; ++i) {
        _logFile << "\nDG " << i + 1 << endl;
        
        _dgList[i]->writeDBG();
        _dgList[i]->merge();
        _dgList[i]->writeDBG();
        _dgList[i]->writeCmdFile();
    }
}

void Mgr::buildDG()
{
    splitLine(_logFile,"Mgr::buildDG()");
    ++_gflag;
    size_t dgIdx = 0;
    PTNodeQueue&   ptq    = _pt->getPTQ();
    Str2DGNodeMap& dgMap  = _pt->getDGMap();
    LCList&        lcList = _pt->getLCList();
    ptq.push(_pt->_root);
    while (!ptq.empty()) {
        dgMap.clear();
        lcList.clear();
        //(_pt->getDGMap()).clear();
        //(_pt->getLCList()).clear();
        PTNode* cur = ptq.front();
        ptq.pop();
        _logFile << "handle " << cur->getName() << endl;
        for (PTNodeList::iterator it=cur->_children.begin();it!=cur->_children.end();++it)
            DGNode* newNode = (*it)->buildDG();
        for (Str2DGNodeMap::iterator it=dgMap.begin(); it!=dgMap.end(); ++it) {
            DGNode* leader = it->second->findLeader();
            if (leader->isSink() && leader->getFlag() != _gflag) {
                _logFile << "found sink node = " << leader->getName() << endl;
                leader->setFlag(_gflag);
                _dgList.push_back(new DG(leader,_indent,_gflag,_path,dgIdx++));
                _lolcList.push_back(lcList);
                //_lolcList.push_back(_pt->getLCList());
            }
        }
    }

}
