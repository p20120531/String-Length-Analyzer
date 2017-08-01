#include "pt.h"
#include "kaluzaMgr.h"
extern       KaluzaMgr*      kmgr;
static       ofstream&       logFile = kmgr->getLogFile();
static const vector<string>& bvList  = kmgr->getBVList();
static const vector<string>& ivList  = kmgr->getIVList();
static const Str2TypeMap&    typeMap = kmgr->getTypeMap();


void PT::printPTNodeListMap() const
{
    #ifndef _NLOG_
    splitLine(logFile,"PT::printPTNodeListMap");
    for (Str2PTNodeListMap::const_iterator it = _ptnodeListMap.begin(); it != _ptnodeListMap.end(); ++it) {
        logFile << it->first << " ";
        for (PTNodeList::const_iterator jt = it->second.begin(); jt != it->second.end(); ++jt)
            logFile << *jt << " ";
        logFile << endl;
    }
    #endif
}
void PT::analyzeASCII()
{
    _root->analyzeASCII();
}

void PT::mergeNotEquivalence()
{
    #ifndef _NLOG_
        splitLine(logFile,"PT::mergeNotEquivalence");
    #endif
    PTNodeQueue ptq;
    ptq.push(_root);
    while (!ptq.empty()) {
        PTNode* cur = ptq.front();
        ptq.pop();
        for (PTNodeList::iterator it = cur->_children.begin(); it != cur->_children.end(); ++it) {
            if ((*it)->_name == "not") {
                assert(((*it)->_children.size() == 1));
                if ((*it)->_children[0]->_name == "str.in.re") {
                    PTNode* newNode = new PTStrNotInReNode("str.nin.re");
                    newNode->_children = (*it)->_children[0]->_children;
                    cur->_children.erase(it);
                    --it;
                    cur->addChild(newNode);
                    ptq.push(newNode);
                }
                else if ((*it)->_children[0]->_name == "=") {
                    PTNode* newNode = new PTNotEqNode("!=");
                    newNode->_children = (*it)->_children[0]->_children;
                    cur->_children.erase(it);
                    --it;
                    cur->addChild(newNode);
                    ptq.push(newNode);
                }
                else {
                    Str2TypeMap::const_iterator jt = typeMap.find((*it)->_children[0]->_name);
                    if (jt != typeMap.end()){
                        if (jt->second != VAR_BOOL) {
                            cout << "[WARNING:PT::mergeNotEquivalence] \"not\" having non-Bool Variable child=" << (*it)->_children[0]->_name << endl;
                        }
                    }
                    else {
                        cout << "[WARNING:PT::mergeNotEquivalence] \"not\" having Bool Return Type child=" << (*it)->_children[0]->_name << " other than Bool Variable" << endl;
                    }
                    ptq.push(*it);
                }
            }
            else 
                ptq.push(*it);
        }
    }
}

void PT::setLevel()
{
    _root->setLevel(0);
}

void PT::buildPTNodeListMap()
{
    #ifndef _NLOG_
        splitLine(logFile,"PT::buildPTNodeListMap");
    #endif
    for (vector<string>::const_iterator it=bvList.begin(); it!=bvList.end(); ++it) {
        _ptnodeListMap.insert(Str2PTNodeList(*it,PTNodeList()));
    }
    for (vector<string>::const_iterator it=ivList.begin(); it!=ivList.end(); ++it) {
        _ptnodeListMap.insert(Str2PTNodeList(*it,PTNodeList()));
    }
    
    PTNodeQueue ptq;
    ptq.push(_root);
    while(!ptq.empty()) {
        PTNode* cur = ptq.front();
        ptq.pop();
        for (PTNodeList::iterator it=cur->_children.begin();it!=cur->_children.end();++it) {
            if ((*it)->_name != "ite") {
                (*it)->buildPTNodeListMap(*it);
            }
            else {
                ptq.push((*it)->_children[1]);
                ptq.push((*it)->_children[2]);
            }
        }
    }
}

string PT::getNewNodeName()
{
    return string("NEW_DGNode_") + itos(++_newDGNodeCnt);
}
