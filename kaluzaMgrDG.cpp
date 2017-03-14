#include "kaluzaMgr.h"

void KaluzaMgr::printDG()
{
    splitLine(_logFile,"KaluzaMgr::printDG");
    _logFile << "dgList = " << _dgList.size() << endl;
    for (size_t i = 0,size = _dgList.size(); i < size; ++i) {
        _logFile << "\nDG " << i + 1 << endl;
        
        _dgList[i]->print();
        _dgList[i]->merge();
        _dgList[i]->print();
        _dgList[i]->writeCmdFile();
    }
}

void KaluzaMgr::buildAndWriteDG()
{
    #ifndef _NLOG_
        splitLine(_logFile,"KaluzaMgr::buildAndWriteDG");
    #endif
    size_t dgIdx = 0;
    PTNodeQueue&   ptq        = _pt->getPTQ();
    Str2DGNodeMap& dgMap      = _pt->getDGMap();
    ptq.push(_pt->_root);
    while (!ptq.empty()) {
        dgMap.clear();
        PTNode* cur = ptq.front();
        ptq.pop();
        #ifndef _NLOG_
            _logFile << "[handle " << cur->getName() << "]" << endl;
        #endif
        ++_gflag;
        for (PTNodeList::iterator it=cur->_children.begin();it!=cur->_children.end();++it)
            DGNode* newNode = (*it)->buildDG();
        size_t sinkCnt = 0,bflag = _gflag;
        for (Str2DGNodeMap::iterator it=dgMap.begin(); it!=dgMap.end(); ++it) {
            DGNode* leader = it->second->findLeader();
            if (leader->isSink() && leader->getFlag() != _gflag) {
                ++sinkCnt;
                DG* dg = new DG(leader,_indent,_gflag,bflag,_path,++dgIdx);
                #ifndef _NLOG_
                    _logFile << "found sink node = " << leader->getName() << endl;
                    _logFile << "\n>> processing DG " << dgIdx << endl;
                #endif
                dg->print();
                dg->merge();
                dg->renameLengthVar();
                dg->writeCVC4File(PTNodeGFlag);
                dg->writeCmdFile();
                dg->writeIC3LengthFile();
            }
        }
        cout << cur->getName() << ": level=" << cur->getLevel() << " sinkCnt=" << sinkCnt << endl;
    }

}
