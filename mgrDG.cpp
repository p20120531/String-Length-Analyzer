#include "mgr.h"

void Mgr::printDG() const
{
    cout << "dgList = " << _dgList.size() << endl;
    for (size_t i = 0,size = _dgList.size(); i < size; ++i) {
        cout << "DG " << i + 1 << endl;
        _dgList[i]->print();
        cout << endl;
    }
}

string Mgr::getNewNodeName(const Type& type)
{
    if ( type == CONST_STRING || type>=13 && type<=17) {
        return string("NEW_STR_").append(uint2str(++_newStrCnt));
    }
    else if (type>=18 && type<=20) {
        return string("NEW_RE_").append(uint2str(++_newReCnt));
    }
}

string Mgr::uint2str(size_t cnt)
{
    stringstream ss;
    ss << cnt;
    return ss.str();
    /*
    size_t tmp = cnt;
    size_t size = 0;
    while( cnt/10 != 0 || cnt%10 != 0 ) {
        ++size;
        cnt /= 10;
    }
    char* c = new char[size];
    for (size_t i = 0; i < size; ++i) {
        cout << "tmp%10+48 = " << char(tmp%10 + 48) << endl;
        c[size-(i+1)] = tmp%10 + 48;
        cout << "c[" << size-(i+1) << "] = " << c[size-(i+1)] << endl;
        tmp /= 10;
    }
    string ret(c,size);
    cout << "result = " << ret << endl;
    return string(c);
    */
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
    for (Str2DGNodeMap::iterator it=dgMap.begin(); it!=dgMap.end(); ++it)
        if (it->second->isSink()) {
            cout << "found sink node = " << it->second->getName() << endl;
            _dgList.push_back(new DG(it->second));
        }
}
