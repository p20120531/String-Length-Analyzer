#include "pt.h"
#include "kaluzaMgr.h"
extern       KaluzaMgr*      kmgr;
static       ofstream&       logFile = kmgr->getLogFile();
static const vector<string>& bvList  = kmgr->getBVList();
static const vector<string>& ivList  = kmgr->getIVList();
static const Str2TypeMap&    typeMap = kmgr->getTypeMap();
void PT::addAssertion(PTNode* n)
{
    _root->addChild(n);
}

void PT::print() const
{
    #ifndef _NLOG_
    splitLine(logFile,"PT::print");
    for (size_t i = 0, size = _root->_children.size(); i < size; ++i) {
        logFile << _root->_name << " " << i + 1 << endl;
        _root->_children[i]->print(_indent,0);
    }
    #endif
}

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
void PT::analyze()
{
    #ifndef _NLOG_
        splitLine(logFile,"PT::analyze");
    #endif
    _iteDVarLegal    = 1;
    _iteCLevel1      = 1;
    _strinreRLevel1  = 1;
    _strninreRLevel1 = 1;
    _streqRLevel1    = 1;
    _strneqRLevel1   = 1;
    _strlenRLevel2   = 1;
    _andCLevel2      = 1;
    _ornexist        = 1;
    _strinreLCSV     = 1;
    _strninreLCSV    = 1;
    _streqLCSV       = 1;
    _strneqLCSV      = 1;
    _strlenCnt       = 0;
    _strlenEqCnt     = 0;
    _streqBothSV     = 0;
    _strneqBothSV    = 0;
    _strneqOneConst  = 1;
    for (size_t i = 0, size = _root->_children.size(); i < size; ++i) {
        _root->_children[i]->analyze(
            _iteDVarLegal,_iteCLevel1,_strinreRLevel1,_strninreRLevel1,
            _streqRLevel1,_strneqRLevel1,_strlenRLevel2,_andCLevel2,_ornexist,
            _strinreLCSV,_strninreLCSV,_streqLCSV,_strneqLCSV,
            _strlenCnt,_strlenEqCnt,_streqBothSV,_strneqBothSV,_strneqOneConst,1,1);
    }
    cout << "   iteDVarLegal=" << _iteDVarLegal 
         << " iteCLevel1=" << _iteCLevel1 
         << " strinreRLevel1=" << _strinreRLevel1
         << " strninreRLevel1=" << _strninreRLevel1
         << " streqRLevel1=" << _streqRLevel1
         << " strneqRLevel1=" << _strneqRLevel1
         << " strlenRLevel2=" << _strlenRLevel2 << endl
         << "   andCLevel2=" << _andCLevel2
         << " ornexist=" << _ornexist
         << " strinreLCSV=" << _strinreLCSV
         << " strninreLCSV=" << _strninreLCSV
         << " streqLCSV=" << _streqLCSV
         << " strneqLCSV=" << _strneqLCSV;
    if (_strlenCnt == _strlenEqCnt)
        cout << "strlenCnt==strlenEqCnt" << endl;
    else
        cout << "strlenCnt!=strlenEqCnt" << endl;
    cout << " streqBothSV=" << _streqBothSV
         << " strneqBothSV=" << _strneqBothSV 
         << " strneqOneConst=" << _strneqOneConst << endl;
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
                    PTNode* newNode = new PTStrNotInReNode("str.notin.re");
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
                            #ifndef _NLOG_
                                logFile << "[WARNING:PT::mergNotEquivalence] \"not\" having non-Bool Variable child=" << (*it)->_children[0]->_name << endl;
                            #endif
                            cout << "[WARNING:PT::mergeNotEquivalence] \"not\" having non-Bool Variable child=" << (*it)->_children[0]->_name << endl;
                        }
                    }
                    else {
                        #ifndef _NLOG_
                            logFile << "[WARNING:PT::mergeNotEquivalence] \"not\" having Bool Return Type child=" << (*it)->_children[0]->_name << " other than Bool Variable" << endl;
                        #endif
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
