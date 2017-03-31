#include "kaluzaMgr.h"

void KaluzaMgr::printDG()
{        
    _dg->print();
}

void KaluzaMgr::buildAndWriteDG()
{
    #ifndef _NLOG_
        splitLine(_logFile,"KaluzaMgr::buildAndWriteDG");
    #endif
    size_t dgIdx = 0;
    PTNodeQueue&   ptq   = _pt->getPTQ();
    IMPQueue&      impq  = _pt->getIMPQ();
    Str2DGNodeMap& dgMap = _pt->getDGMap();
    ptq.push(_pt->_root);
    impq.push(IMP(0,0));
    while (!ptq.empty()) {
        dgMap.clear();
        PTNode* cur = ptq.front();
        IMP curimp = impq.front();
        ptq.pop();
        impq.pop();
        #ifndef _NLOG_
            _logFile << "[handle " << cur->getName() << "]" << endl;
        #endif
        ++_gflag;
        for (PTNodeList::iterator it=cur->_children.begin();it!=cur->_children.end();++it)
            DGNode* newNode = (*it)->buildDG();
        size_t bflag = _gflag;
        
        ++_gflag;
        DGNodeList sinkList;
        for (Str2DGNodeMap::iterator it=dgMap.begin(); it!=dgMap.end(); ++it) {
            DGNode* leader = it->second->findLeader();
            if (leader->isSink() && leader->getFlag() != _gflag) {
                leader->setFlag(_gflag);
                sinkList.push_back(leader);
            }
        }

        for (DGNodeList::iterator it=sinkList.begin(); it!=sinkList.end(); ++it) {
            _dg = new DG(*it,_indent,_gflag,bflag,_path,++dgIdx);
            #ifndef _NLOG_
                _logFile << "found sink node = " << (*it)->getName() << endl;
                _logFile << "\n>> processing DG " << dgIdx << endl;
            #endif
            _dg->print();
            ++_gflag;
            _dg->merge();
            //_dg->print();
            //_dg->printLengthVarList();
            _dg->renameLengthVar();
            _dg->print();
            ++_gflag;
            _dg->writeCVC4File(curimp);
            ++_gflag;
            _dg->writeCmdFile();
            //dg->writeIC3LengthFile();
        }
        //cout << cur->getName() << ": level=" << cur->getLevel() << " sinkCnt=" << sinkList.size() << endl;
    }

}
