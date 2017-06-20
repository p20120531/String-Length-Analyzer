#include "autMgr.h"
#include <map>

extern AutMgr* autmgr;
static size_t& gflag = autmgr->getGFlag();

string VmtNode::getTypeStr() 
{
    string typeStr;
    switch (_type) {
        case INPUT   : typeStr = "INPUT";
                       break;
        case EXIST   : typeStr = "EXIST";
                       break;
        case STATE   : typeStr = "STATE";
                       break;
        case LEN     : typeStr = "LEN";
                       break;
        case STATE_N : typeStr = "STATE_N";
                       break;
        case LEN_N   : typeStr = "LEN_N";
                       break;
        case PARAM   : typeStr = "PARAM";
                       break;
        case OTHER   : typeStr = "OTHER";
                       break;
        default      : typeStr = "INVALID";
                       break;
    }
    return typeStr;
}

void VmtNode::print(const size_t& level) 
{
    cout << string(level*3,' ') << _name << " " << this << " " << getTypeStr() << " " << _flag;
    for (size_t i = 0; i < 6; ++i) {
        for (size_t j = 0, size = _paramList[i].size(); j < size; ++j)
            cout << " " << _paramList[i][j]->_name;
    }
    cout << endl;
    for (size_t i = 0, size = _children.size(); i < size; ++i) {
        _children[i]->print(level+1);
    }
}

void VmtNode::write(const size_t& level,ofstream& outFile)
{
    #ifndef VMTNODE_NDEBUG
        cout << string(level*3,' ') << _name << " " << this << " " << getTypeStr() << " " << _flag;
        if      (_flag == gflag)     cout << " visited";
        else                         cout << " unvisited";
        if      (_type == PARAM)     cout << " PARAM\n";
        else if (!_children.empty()) cout << " internal\n";
        else                         cout << " leaf\n";
    #endif
    if (_flag == gflag) {
        if (_type == PARAM) {
            assert( (_children.empty()) );
            assert( (_source->_flag == gflag) );
            assert( (hasParam()) );
            outFile << "(" << _source->_name;
            for (size_t i = 0; i < 6; ++i) {
                for (size_t j = 0, size = _paramList[i].size(); j < size; ++j)
                    outFile << " " << _paramList[i][j]->_name;
            }
            outFile << ")";
        }
        else {
            if (!_children.empty()) {
                if ( hasParam() ) {
                    outFile << "(" << _name;
                    for (size_t i = 0; i < 6; ++i) {
                        for (size_t j = 0, size = _paramList[i].size(); j < size; ++j)
                            outFile << " " << _paramList[i][j]->_name;
                    }
                    outFile << ")";
                }
                else {
                    cout << "[VmtNode::write] special node name=" << _name << " c0=" << _children[0]->_name << endl;
                    assert( (_children.size() == 1) );
                    assert( (_children[0]->_name == "true" || _children[0]->_name == "false") );
                    outFile << _name;
                }
            }
            else {
                outFile << _name;
            }
        }
        return;
    }
    _flag = gflag;
    if (_type == PARAM) {
        assert( (_children.empty()) );
        assert( (_source->_flag == gflag) );
        assert( (hasParam()) );
        outFile << "(" << _source->_name;
        for (size_t i = 0; i < 6; ++i) {
            for (size_t j = 0, size = _paramList[i].size(); j < size; ++j)
                outFile << " " << _paramList[i][j]->_name;
        }
        outFile << ")";
    }
    else {
        if (!_children.empty()) {
            outFile << "(" << _name;
            for (size_t i = 0, size = _children.size(); i < size; ++i) {
                outFile << " ";
                _children[i]->write(level+1,outFile);
            }
            outFile << ")";
        }
        else {
            outFile << _name;
        }
    }
}

bool VmtNode::hasParam()
{
    for (size_t i = 0; i < 6; ++i) {
        if (!_paramList[i].empty())
            return 1;
    }
    return 0;
}

bool VmtNode::haveSameParam(VmtNode* n)
{
    for (size_t i = 0; i < 6; ++i) {
        set<string> s0,s1;
        for (size_t j = 0, size = _paramList[i].size(); j < size; ++j)
            s0.insert(_paramList[i][j]->_name);
        for (size_t j = 0, size = n->_paramList[i].size(); j < size; ++j)
            s1.insert(n->_paramList[i][j]->_name);
        for (set<string>::iterator it = s1.begin(); it != s1.end(); ++it)
            if (s0.find(*it) == s0.end())
                return 0;
    }
    return 1;
}

void VmtNode::addChild(VmtNode* n)
{
    _children.push_back(n);
}

void VmtNode::clearParam(const size_t& level)
{
    #ifndef VMTNODE_NDEBUG
        cout << string(level*3,' ') << _name << " " << this << " " << getTypeStr() << " " << _flag;
        if      (_flag == gflag)   cout << " visited";
        else if (_type == PARAM)   cout << " PARAM";
        cout << endl;
    #endif
    if (_flag == gflag || _type == PARAM) return;
    
    _flag = gflag;
    
    for (size_t i = 0; i < 6; ++i)
        _paramList[i].clear();
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->clearParam(level+1);
}

void VmtNode::buildParam(const size_t& level)
{
    #ifndef VMTNODE_NDEBUG
        cout << string(level*3,' ') << _name << " " << this << " " << getTypeStr() << " " << _flag;
        if      (_flag == gflag)   cout << " visited";
        else if (_type == PARAM)   cout << " PARAM";
        cout << endl;
    #endif
    if (_flag == gflag || _type == PARAM) return;
    
    _flag = gflag;
    if (_children.empty()) {
        switch (_type) {
            case INPUT   :
                _paramList[0].push_back(this);
                break;
            case EXIST   :
                _paramList[1].push_back(this);
                break;
            case STATE :
                _paramList[2].push_back(this);
                break;
            case LEN   :
                _paramList[3].push_back(this);
                break;
            case STATE_N :
                _paramList[4].push_back(this);
                break;
            case LEN_N   :
                _paramList[5].push_back(this);
                break;
            default :
                break;
        }
    }
    else {
        vector<set<size_t> > count(6,set<size_t>());
        for (size_t i = 0, size = _children.size(); i < size; ++i) {
            _children[i]->buildParam(level+1);
            for (size_t j = 0; j < 6; ++j) {
                for (size_t k = 0, size = _children[i]->_paramList[j].size(); k < size; ++k) {
                    count[j].insert(_children[i]->_paramList[j][k]->_idx);
                }
            }
        }
        //if (!count[0].empty()) assert( (count[1].empty()) );
        //if (!count[1].empty()) assert( (count[0].empty()) );
        //assert( (count[2].size() == count[4].size()) );
        //assert( (count[3].size() == count[5].size()) );
        
        for (set<size_t>::iterator it = count[0].begin(); it != count[0].end(); ++it) {
            _paramList[0].push_back(Aut::input[0][*it]);
        }
        
        for (set<size_t>::iterator it = count[1].begin(); it != count[1].end(); ++it) {
            _paramList[1].push_back(Aut::evar[0][*it]);
        }
        
        for (set<size_t>::iterator it = count[2].begin(); it != count[2].end(); ++it) {
            _paramList[2].push_back(Aut::state[0][*it]);
        }
        
        for (set<size_t>::iterator it = count[3].begin(); it != count[3].end(); ++it) {
            _paramList[3].push_back(Aut::lvar[0][*it]);
        }
        
        for (set<size_t>::iterator it = count[4].begin(); it != count[4].end(); ++it) {
            _paramList[4].push_back(Aut::state[1][*it]);
        }
        
        for (set<size_t>::iterator it = count[5].begin(); it != count[5].end(); ++it) {
            _paramList[5].push_back(Aut::lvar[1][*it]);
        }
        /*
        for (size_t i = 0, size = _paramList[2].size(); i < size; ++i)
            assert( (_paramList[2][i]->_idx == _paramList[4][i]->_idx) );
        for (size_t i = 0, size = _paramList[3].size(); i < size; ++i)
            assert( (_paramList[3][i]->_idx == _paramList[5][i]->_idx) );
        */
    }
}

void VmtNode::collectPARAM(VmtNodeList& PARAMList)
{
    if (_flag == gflag) return;
    _flag = gflag;
    if (_type == PARAM) PARAMList.push_back(this);
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->collectPARAM(PARAMList);
}

void VmtNode::writeParam(ofstream& file)
{
    bool isfirst = 1;
    #ifndef VMTNODE_NDEBUG
        cout << "[VmtNode::writeParam]";
    #endif
    for (size_t i = 0; i < 6; ++i) {
        for (size_t j = 0, size = _paramList[i].size(); j < size; ++j) {
            #ifndef VMTNODE_NDEBUG
                cout << " " << _paramList[i][j]->_name << " " << _paramList[i][j]->getTypeStr();
            #endif
            if (!isfirst) file << " ";
            isfirst = 0;
            file << "(" << _paramList[i][j]->_name;
            if (i == 3 || i == 5) {
                file << " Int)";
            }
            else {
                file << " Bool)";
            }
        }
    }
    #ifndef VMTNODE_NDEBUG
        cout << endl;
    #endif
}

void VmtNode::shiftStateVar(const size_t& delta)
{
    //cout << "node=" << _name <<endl;
    if (_flag == gflag) return;
    /*
    cout << "node=" << _name << " unvisited"<<endl;
    for (size_t i=0;i<6;++i){
        for (VmtNodeSet::iterator it=_paramList[i].begin();it!=_paramList[i].end();++it)
            cout << (*it)->getName() << endl;
    }
    */
    _flag = gflag;
    if (_type == PARAM) {
        assert((_children.empty()));
        
        VmtNodeList tmp;
        for (size_t i = 0, size = _paramList[2].size(); i < size; ++i)
            tmp.push_back( Aut::state[0][ _paramList[2][i]->_idx + delta ] );
        _paramList[2] = tmp;
        
        tmp.clear();
        for (size_t i = 0, size = _paramList[4].size(); i < size; ++i)
            tmp.push_back( Aut::state[1][ _paramList[4][i]->_idx + delta ] );
        _paramList[4] = tmp;
        return;
    }

    for (size_t i = 0, size = _children.size(); i < size; ++i) {
        _children[i]->shiftStateVar(delta);
        //cout << "node = " << _name << " children[" << i << "] = " << _children[i]->_name << endl;
        size_t idx = _children[i]->_idx + delta;
        if (_children[i]->_type == STATE) {
            //cout << "renameSV " << _children[i]->_idx << " -> " <<  xsList[1][svar]->_idx << endl;
            _children[i] = Aut::state[0][idx];
        }
        else if (_children[i]->_type == STATE_N) {
            //cout << "renameSV " << _children[i]->_idx << " -> " <<  xsList[3][svar]->_idx << endl;
            _children[i] = Aut::state[1][idx];
        }
    }
}
