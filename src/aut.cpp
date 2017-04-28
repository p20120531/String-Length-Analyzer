#include "autopMgr.h"
#include <map>
//#define _NPARAM_

extern AutOpMgr* autopmgr;
static vector<VmtNodeList> xsList = autopmgr->_xsList;
static VmtNode* epsilon = autopmgr->_epsilon;
static size_t stateBitNum = autopmgr->_stateBitNum;
static size_t gflag = autopmgr->_gflag;

void VmtNode::addChild(VmtNode* n)
{
    _children.push_back(n);
}

void VmtNode::print(const size_t level) 
{
    cout << string(level,' ') << _name << endl;
    for (size_t i=0,size=_children.size(); i<size; ++i) {
        _children[i]->print(level+1);
    }
}

void VmtNode::write(ofstream& outFile)
{
    
    if (_flag == gflag) {
        #ifndef _NPARAM_
        if (hasParam() && !_children.empty()) {
            outFile << "(" << _name;
            for (size_t i = 0; i < 6; ++i) {
                for (VmtNodeSet::iterator it=_paraList[i].begin();it!=_paraList[i].end();++it) {
                    outFile << " " << (*it)->_name;
                }
            }
            outFile << ")";
        }
        else {
            outFile << _name;
        }
        return;
        #else
        outFile << _name;
        return;
        #endif
    }
    _flag = gflag;
    if (_type == PARAM) {
        assert((_children.empty()));
        outFile << "(" << _source->_name;
        for (size_t i = 0; i < 6; ++i) {
            for (VmtNodeSet::iterator it=_paraList[i].begin();it!=_paraList[i].end();++it) {
                outFile << " " << (*it)->_name;
            }
        }
        outFile << ")";
    }
    else {
        if (!_children.empty()) {
            outFile << "(" << _name;
            for (size_t i = 0, size = _children.size(); i < size; ++i) {
                outFile << " ";
                _children[i]->write(outFile);
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
        if (!_paraList[i].empty())
            return 1;
    }
    return 0;
}

bool VmtNode::haveSameParam(VmtNode* n)
{
    for (size_t i = 0; i < 6; ++i)
        for (VmtNodeSet::iterator it=n->_paraList[i].begin(); it!=n->_paraList[i].end(); ++it) {
            VmtNodeSet::iterator jt = _paraList[i].find(*it);
            if (jt == _paraList[i].end()) return 0;
        }
    return 1;
}

void VmtNode::buildParam()
{
    if (_flag == gflag || _type == NOPARAM) return;
    _flag = gflag;
    if (_children.empty()) {
        if (_type != PARAM) {
            //cout << " clear paralist" << endl;
            for (size_t i = 0; i < 6; ++i)
                _paraList[i].clear();
        }
        switch (_type) {
            case INPUT   :
                _paraList[0].insert(this);
                break;
            case STATE :
                _paraList[1].insert(this);
                break;
            case LEN   :
                _paraList[2].insert(this);
                break;
            case INPUT_N :
                _paraList[3].insert(this);
                break;
            case STATE_N :
                _paraList[4].insert(this);
                break;
            case LEN_N   :
                _paraList[5].insert(this);
                break;
            case PARAM   :
            case OTHER   :
            default      :
                break;
        }
    }
    else {
        for (size_t i = 0,size = _children.size(); i < size; ++i) {
            _children[i]->buildParam();
            for (size_t j = 0; j < 6; ++j)
                merge(_paraList[j],_children[i]->_paraList[j]);
        }
    }
}

// Merge v2 into v1 with repetition eliminated
void VmtNode::merge(VmtNodeSet& v1, VmtNodeSet& v2) 
{
    for (VmtNodeSet::iterator it = v2.begin(); it != v2.end(); ++it)
        v1.insert(*it);
}

void VmtNode::writeParam(ofstream& file)
{
    bool isfirst = 1;
    for (size_t j = 0; j < 6; ++j) {
        for (VmtNodeSet::iterator it=_paraList[j].begin(); it != _paraList[j].end(); ++it) {
            if (!isfirst) file << " ";
            isfirst = 0;
            file << "(" << (*it)->_name;
            if (j == 2 || j == 5) {
                file << " Int)";
            }
            else {
                file << " Bool)";
            }
        }
    }
}

void VmtNode::renameStateVar(const size_t& delta)
{
    //cout << "node=" << _name <<endl;
    if (_flag == gflag) return;
    //cout << "node=" << _name << " unvisited"<<endl;
    _flag = gflag;
    if (_type == PARAM) {
        assert((_children.empty()));
        size_t n1 = (*_paraList[1].begin())->_num;
        size_t n2 = (*_paraList[1].rbegin())->_num;
        _paraList[1].clear();
        assert((n2+delta) < stateBitNum);
        for (size_t i=n1+delta; i<n2+delta+1; ++i)
            _paraList[1].insert(xsList[1][i]);
        
        n1 = (*_paraList[4].begin())->_num;
        n2 = (*_paraList[4].rbegin())->_num;
        _paraList[4].clear();
        assert((n2+delta) < stateBitNum);
        for (size_t i=n1+delta; i<n2+delta+1; ++i)
            _paraList[4].insert(xsList[3][i]);
        return;
    }
    for (size_t i=0,size=_children.size();i<size;++i) {
        _children[i]->renameStateVar(delta);
        if (_children[i]->_type == STATE) {
            assert((_children[i]->_num + delta < stateBitNum));
            _children[i] = xsList[1][_children[i]->_num+delta];
        }
        else if (_children[i]->_type == STATE_N) {
            assert((_children[i]->_num + delta < stateBitNum));
            _children[i] = xsList[3][_children[i]->_num+delta];
        }
    }
}

void Aut::insertXSListAndEpsilon2vmap()
{
    _vmap.insert(Str2VmtNode("epsilon",epsilon));
    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0,size = xsList[i].size(); j < size; ++j)
            _vmap.insert(Str2VmtNode(xsList[i][j]->_name,xsList[i][j]));
    }
}

void Aut::parse(const char* fileName)
{
    string line;
    ifstream file(fileName);

    if (!file) {
        cout << "fail open file=" << fileName << endl;
        return;
    }
    
    vector<string> tokenList,paramList;
    int  sCnt = 0;
    bool svarbuild = 0;
    while (getline(file,line)) {
        if (line[0] == ';') {
            ++sCnt;
            continue;
        }
        tokenList.clear();
        paramList.clear();
        //cout << "line=" << line << endl;
        vmtTokenize(line,paramList,tokenList);
        //cout << "parse::       " << tokenList[0] << "|" << tokenList[1] << "|" <<endl;
        if (tokenList[0] == "epsilon") continue;
        if (sCnt == 0) {
            if (tokenList[0][0] == 's') {
                ++_stateVarCnt;
            }
            else if (tokenList[0][0] == 'n'){
                VmtNode* newNode = 0;
                if (*(tokenList[0].rbegin()) == 't') {
                    size_t num = stoi(tokenList[0].substr(1,tokenList[0].find_last_of(".")-1));
                    //cout << "n.next num=" << num << endl;
                    newNode = new VmtNode(tokenList[0],LEN_N,num);
                    _snList[3].push_back(newNode);
                }
                else {
                    size_t num = stoi(tokenList[0].substr(1));
                    //cout << "n num=" << num << endl;
                    newNode = new VmtNode(tokenList[0],LEN,num);
                    _snList[1].push_back(newNode);
                }
                _vmap.insert(Str2VmtNode(tokenList[0],newNode));
            }
        }
        else {
            if (!svarbuild) {
                // Add epsilon node
                /*
                VmtNode* eNode = new VmtNode("epsilon");
                for (size_t i=0; i<8; ++i) {
                    VmtNode* notNode = new VmtNode("not");
                    notNode->addChild(xsList[0][i]);
                    eNode->addChild(notNode);
                }
                _imdList.push_back(eNode);
                */
                svarbuild = 1;
                assert((_stateVarCnt%2 == 0));
                _stateVarCnt /= 2;
                assert((_stateVarCnt <= stateBitNum));
                for (size_t i = 0; i < _stateVarCnt; ++i) {
                    _snList[0].push_back(xsList[1][i]);
                    _snList[2].push_back(xsList[3][i]);
                }
            }
            //cout << tokenList[0] << "|" << tokenList[1] << "|" <<endl;
            VmtNode* newNode1 = new VmtNode(tokenList[0]);
            VmtNode* newNode2 = buildVmtNode(tokenList[1],0,tokenList[1].size(),_vmap);
            newNode1->addChild(newNode2);
            for (vector<string>::iterator it=paramList.begin(); it!=paramList.end(); ++it) {
                Str2VmtNodeMap::iterator jt = _vmap.find(*it);
                assert( (jt != _vmap.end()) );
                const VmtType& type = jt->second->_type;
                switch (type) {
                    case INPUT   :
                        newNode1->_paraList[0].insert(jt->second);
                        break;
                    case STATE   :
                        newNode1->_paraList[1].insert(jt->second);
                        break;
                    case LEN     :
                        newNode1->_paraList[2].insert(jt->second);
                        break;
                    case INPUT_N :
                        newNode1->_paraList[3].insert(jt->second);
                        break;
                    case STATE_N :
                        newNode1->_paraList[4].insert(jt->second);
                        break;
                    case LEN_N   :
                        newNode1->_paraList[5].insert(jt->second);
                        break;
                    default    :
                        break;
                }
            }
            //cout << "n1=" << newNode1->_name << " n2=" << newNode2->_name << endl;
            if (sCnt != 1) _vmap.insert(Str2VmtNode(tokenList[0],newNode1));
            if      (sCnt == 1) _nxtList.push_back(newNode1);
            else if (sCnt == 2) _imdList.push_back(newNode1);
            else if (sCnt == 3) _itoList.push_back(newNode1);
        }
    }
    file.close();
}

void Aut::write(const string& fileName)
{
    write(fileName.c_str());
}

void Aut::write(const char* fileName)
{
    ofstream file(fileName);
    buildParam();
    ++gflag;

    for (size_t i = 0; i < 2; ++i) {
        for (VmtNodeList::iterator it=xsList[i*2].begin(); it!=xsList[i*2].end(); ++it) {
            (*it)->_flag = gflag;
            file << "(declare-fun " << (*it)->_name << " () Bool)\n";
        }
    }

    for (size_t i = 0; i < 4; ++i) {
        for (VmtNodeList::iterator it=_snList[i].begin(); it!=_snList[i].end(); ++it) {
            (*it)->_flag = gflag;
            file << "(declare-fun " << (*it)->_name;
            if ((*it)->_type == LEN || (*it)->_type == LEN_N) {
                file << " () Int)\n";
            }
            else {
                file << " () Bool)\n";
            }
        }
    }
    file << ";\n";
    
    for (size_t i = 0,size = _nxtList.size(); i < size; ++i) {
        file << "(define-fun " 
             << _nxtList[i]->_name
             << " () Bool ";
        _nxtList[i]->_flag = gflag;
        _nxtList[i]->_children[0]->write(file);
        file << ")\n";
    }
    file << ";\n";
    
    #ifndef _NPARAM_
    file << "(define-fun epsilon ((x0 Bool) (x1 Bool) (x2 Bool) (x3 Bool) (x4 Bool) (x5 Bool) (x6 Bool) (x7 Bool)) Bool (and (not x0) (not x1) (not x2) (not x3) (not x4) (not x5) (not x6) (not x7)))\n";
    #else
    file << "(define-fun epsilon () Bool (and (not x0) (not x1) (not x2) (not x3) (not x4) (not x5) (not x6) (not x7)))\n";
    #endif

    epsilon->_flag = gflag;

    for (size_t i = 0,size = _imdList.size(); i < size; ++i) {
        file << "(define-fun "
             << _imdList[i]->_name
             << " (";
        #ifndef _NPARAM_
        _imdList[i]->writeParam(file);
        #endif
        file << ") Bool ";
        _imdList[i]->_flag = gflag;
        _imdList[i]->_children[0]->write(file);
        file << ")\n";
    }
    file << ";\n";

    for (size_t i = 0,size = _itoList.size(); i < size; ++i) {
        file << "(define-fun " 
             << _itoList[i]->_name
             << " () Bool ";
        _itoList[i]->_flag = gflag;
        _itoList[i]->_children[0]->write(file);
        file << ")\n";
    }
    file.close();
}

void Aut::addlen(const string& lcvarCnt)
{

    // Construct Length Variable
    string lcvar  = "n" + lcvarCnt;
    string lcvarn = lcvar + ".next";
    
    // Add Length Variable
    size_t lcnum = stoi(lcvarCnt);
    for (VmtNodeList::iterator it=_snList[1].begin(); it!=_snList[1].end(); ++it) assert((*it)->_num != lcnum);
    for (VmtNodeList::iterator it=_snList[3].begin(); it!=_snList[3].end(); ++it) assert((*it)->_num != lcnum);
    
    VmtNode* newNode1 = new VmtNode(lcvar,LEN,lcnum);
    VmtNode* newNode2 = new VmtNode(lcvarn,LEN_N,lcnum);
    _vmap.insert(Str2VmtNode(lcvar,newNode1));
    _vmap.insert(Str2VmtNode(lcvarn,newNode2));
    _snList[1].push_back(newNode1);
    _snList[3].push_back(newNode2);

    // Add Next State
    string snxt = "(! " + lcvar + " :next " + lcvarn + ")";
    newNode1 = new VmtNode("tmpNxt");
    newNode1->addChild(buildVmtNode(snxt,0,snxt.size(),_vmap));
    _nxtList.push_back(newNode1);

    // Add Length Constraint
    string s1 = "(= " + lcvarn + " (+ " + lcvar + " 1))";
    string s2 = "(= " + lcvarn + " " + lcvar + ")";
    newNode1 = new VmtNode("tmp1");
    newNode2 = new VmtNode("tmp2");
    _vmap.insert(Str2VmtNode("tmp1",newNode1));
    _vmap.insert(Str2VmtNode("tmp2",newNode2));
    newNode1->addChild(buildVmtNode(s1,0,s1.size(),_vmap));
    newNode2->addChild(buildVmtNode(s2,0,s2.size(),_vmap));
    
    VmtNode* tNode = getT();
    VmtNode* newNode3 = new VmtNode("tmp3");
    string s3 = "(and " + tNode->_name + " (or (and (not epsilon) tmp1) (and epsilon tmp2)))";
    newNode3->addChild(buildVmtNode(s3,0,s3.size(),_vmap));

    _imdList.insert(_imdList.begin(),newNode2);
    _imdList.insert(_imdList.begin(),newNode1);
    _imdList.push_back(newNode3);

    setT(newNode3);
    renameDef();
}

void Aut::intersect(Aut* a1,Aut* a2)
{
    a2->renameStateVar(a1->_stateVarCnt);
    
    _stateVarCnt = a1->_stateVarCnt + a2->_stateVarCnt;
    
    Str2VmtNodeMap vmap;
    vmap.insert(Str2VmtNode("epsilon",epsilon));
    for (size_t i=0; i<_stateVarCnt; ++i) {
        vmap.insert(Str2VmtNode(xsList[1][i]->_name,xsList[1][i]));
        vmap.insert(Str2VmtNode(xsList[3][i]->_name,xsList[3][i]));
    }
    
    // Add Epsilon Loop
    VmtNode* newNode1 = new VmtNode("tmp1");
    string s1 = "(and epsilon";
    for (size_t i = 0,size=a1->_snList[0].size(); i < size; ++i) {
        s1 += " (= " + a1->_snList[2][i]->_name + " " + a1->_snList[0][i]->_name + ")";
    }
    s1 += ")";
    newNode1->addChild(buildVmtNode(s1,0,s1.size(),vmap));
    
    VmtNode* newNode2 = new VmtNode("tmp2");
    string s2 = "(and epsilon";
    for (size_t i = 0,size=a2->_snList[0].size(); i < size; ++i) {
        s2 += " (= " + a2->_snList[2][i]->_name + " " + a2->_snList[0][i]->_name + ")";
    }
    s2 += ")";
    newNode2->addChild(buildVmtNode(s2,0,s2.size(),vmap));

    vmap.insert(Str2VmtNode("tmp1",newNode1));
    vmap.insert(Str2VmtNode("tmp2",newNode2));
    
    // Transition
    VmtNode* t1 = a1->getT();
    VmtNode* t2 = a2->getT();
    t1->_name = "tmpT1";
    t2->_name = "tmpT2";
    vmap.insert(Str2VmtNode("tmpT1",t1));
    vmap.insert(Str2VmtNode("tmpT2",t2));
    VmtNode* newNode3 = new VmtNode("tmp3");
    string s3 = "(and (or tmpT1 tmp1) (or tmpT2 tmp2))";
    newNode3->addChild(buildVmtNode(s3,0,s3.size(),vmap));

    VmtNode* tNode = new VmtNode("T");
    vmap.insert(Str2VmtNode("tmp3",newNode3));
    string s6 = "(! tmp3 :trans true)";
    tNode->addChild(buildVmtNode(s6,0,s6.size(),vmap));

    // Initial
    VmtNode* i1 = a1->getI();
    VmtNode* i2 = a2->getI();
    i1->_name = "tmpI1";
    i2->_name = "tmpI2";
    vmap.insert(Str2VmtNode("tmpI1",i1));
    vmap.insert(Str2VmtNode("tmpI2",i2));
    VmtNode* newNode4 = new VmtNode("tmp4");
    string s4 = "(and tmpI1 tmpI2)";
    newNode4->addChild(buildVmtNode(s4,0,s4.size(),vmap));
    
    VmtNode* iNode = new VmtNode("I");
    vmap.insert(Str2VmtNode("tmp4",newNode4));
    string s7 = "(! tmp4 :init true)";
    iNode->addChild(buildVmtNode(s7,0,s7.size(),vmap));

    // Accepting
    VmtNode* o1 = a1->getO();
    VmtNode* o2 = a2->getO();
    o1->_name = "tmpO1";
    o2->_name = "tmpO2";
    vmap.insert(Str2VmtNode("tmpO1",o1));
    vmap.insert(Str2VmtNode("tmpO2",o2));
    VmtNode* newNode5 = new VmtNode("tmp5");
    string s5 = "(and tmpO1 tmpO2)";
    newNode5->addChild(buildVmtNode(s5,0,s5.size(),vmap));

    VmtNode* oNode = new VmtNode("O");
    vmap.insert(Str2VmtNode("tmp5",newNode5));
    string s8 = "(! (not tmp5) :invar-property 0)";
    oNode->addChild(buildVmtNode(s8,0,s8.size(),vmap));

    // Merge List
    for (size_t i=0; i<_stateVarCnt; ++i) {
        _snList[0].push_back(xsList[1][i]);
        _snList[2].push_back(xsList[3][i]);
    }
    
    _snList[1] = a1->_snList[1];
    _snList[3] = a1->_snList[3];
    for (size_t i=0,size1=a2->_snList[1].size(); i<size1; ++i) {
        for (size_t j=0,size2=_snList[1].size(); j<size2; ++j)
            assert((_snList[1][j]->_num != a2->_snList[1][i]->_num));
        _snList[1].push_back(a2->_snList[1][i]);
    }
    for (size_t i=0,size1=a2->_snList[3].size(); i<size1; ++i) {
        for (size_t j=0,size2=_snList[3].size(); j<size2; ++j)
            assert((_snList[3][j]->_num != a2->_snList[3][i]->_num));
        _snList[3].push_back(a2->_snList[3][i]);
    }
        
    _nxtList = a1->_nxtList;
    for (size_t i=0,size=a2->_nxtList.size(); i<size; ++i) {
        if (a2->_nxtList[i]->_children[0]->_children[0]->_name[0] == 'x') continue;
        _nxtList.push_back(a2->_nxtList[i]);
    }

    _imdList = a1->_imdList;
    for (size_t i=0,size=a2->_imdList.size(); i<size; ++i) {
        _imdList.push_back(a2->_imdList[i]);
    }

    _imdList.insert(_imdList.begin(),newNode2);
    _imdList.insert(_imdList.begin(),newNode1);
    _imdList.push_back(newNode3);
    _imdList.push_back(newNode4);
    _imdList.push_back(newNode5);

    _itoList.push_back(iNode);
    _itoList.push_back(tNode);
    _itoList.push_back(oNode);

    renameDef();
}

void Aut::concate(Aut* a1,Aut* a2)
{
    _stateVarCnt = (a1->_stateVarCnt > a2->_stateVarCnt)? a1->_stateVarCnt : a2->_stateVarCnt;
    ++_stateVarCnt;
    assert((_stateVarCnt <= stateBitNum));
    string sa  = xsList[1][_stateVarCnt-1]->_name;
    string san = xsList[3][_stateVarCnt-1]->_name;

    Str2VmtNodeMap vmap;
   
    vmap.insert(Str2VmtNode("epsilon",epsilon));
    vmap.insert(Str2VmtNode(sa,xsList[1][_stateVarCnt-1]));
    vmap.insert(Str2VmtNode(san,xsList[3][_stateVarCnt-1]));

    assert((a1->_snList[1].size() == a1->_snList[3].size()));
    assert((a2->_snList[1].size() == a2->_snList[3].size()));
    for (size_t i=0,size=a1->_snList[1].size(); i<size; ++i) {
        vmap.insert(Str2VmtNode(a1->_snList[1][i]->_name,a1->_snList[1][i]));
        vmap.insert(Str2VmtNode(a1->_snList[3][i]->_name,a1->_snList[3][i]));
    }
    for (size_t i=0,size=a2->_snList[1].size(); i<size; ++i) {
        vmap.insert(Str2VmtNode(a2->_snList[1][i]->_name,a2->_snList[1][i]));
        vmap.insert(Str2VmtNode(a2->_snList[3][i]->_name,a2->_snList[3][i]));
    }

    VmtNode* i1 = a1->getI();
    VmtNode* i2 = a2->getI();
    i1->_name = "tmpI1";
    i2->_name = "tmpI2";
    vmap.insert(Str2VmtNode("tmpI1",i1));
    vmap.insert(Str2VmtNode("tmpI2",i2));
    
    VmtNode* o1 = a1->getO();
    VmtNode* o2 = a2->getO();
    o1->_name = "tmpO1";
    o2->_name = "tmpO2";
    vmap.insert(Str2VmtNode("tmpO1",o1));
    vmap.insert(Str2VmtNode("tmpO2",o2));
    
    VmtNode* t1 = a1->getT();
    VmtNode* t2 = a2->getT();
    t1->_name = "tmpT1";
    t2->_name = "tmpT2";
    vmap.insert(Str2VmtNode("tmpT1",t1));
    vmap.insert(Str2VmtNode("tmpT2",t2));
    
    // Transition
    VmtNode* newNode1 = new VmtNode("tmp1");
    string s1 = "(and (not " + sa + ") (not " + san + ") tmpT1";
    for (size_t i=0, size=a2->_snList[1].size(); i<size; ++i) {
        s1 += " (= " + a2->_snList[3][i]->_name + " " + a2->_snList[1][i]->_name + ")";
    }
    s1 += ")"; 
    newNode1->addChild(buildVmtNode(s1,0,s1.size(),vmap));

    VmtNode* newNode2 = new VmtNode("tmp2");
    string s2 = "(and " + sa + " " + san + " tmpT2";
    for (size_t i=0, size=a1->_snList[1].size(); i<size; ++i) {
        s2 += " (= " + a1->_snList[3][i]->_name + " " + a1->_snList[1][i]->_name + ")";
    }
    s2 += ")"; 
    newNode2->addChild(buildVmtNode(s2,0,s2.size(),vmap));

    VmtNode* newNodeI2 = new VmtNode("I2_PARAM",PARAM);
    newNodeI2->_source = i2;
    vmap.insert(Str2VmtNode("I2_PARAM",newNodeI2));
    for (size_t i=0,size=a2->_snList[2].size(); i<size; ++i)
        newNodeI2->_paraList[4].insert(a2->_snList[2][i]);

    VmtNode* newNode3 = new VmtNode("tmp3");
    string s3 = "(and epsilon (not " + sa + ") " + san + " tmpO1 I2_PARAM";
    for (size_t i=0, size=a1->_snList[1].size(); i<size; ++i) {
        s3 += " (= " + a1->_snList[3][i]->_name + " " + a1->_snList[1][i]->_name + ")";
    }
    for (size_t i=0, size=a2->_snList[1].size(); i<size; ++i) {
        s3 += " (= " + a2->_snList[3][i]->_name + " " + a2->_snList[1][i]->_name + ")";
    }
    s3 += ")"; 
    newNode3->addChild(buildVmtNode(s3,0,s3.size(),vmap));

    vmap.insert(Str2VmtNode("tmp1",newNode1));
    vmap.insert(Str2VmtNode("tmp2",newNode2));
    vmap.insert(Str2VmtNode("tmp3",newNode3));
    VmtNode* newNode4 = new VmtNode("tmp4");
    string s4 = "(or tmp1 tmp2 tmp3)";
    newNode4->addChild(buildVmtNode(s4,0,s4.size(),vmap));

    vmap.insert(Str2VmtNode("tmp4",newNode4));
    VmtNode* tNode = new VmtNode("T");
    string sT = "(! tmp4 :trans true)";
    tNode->addChild(buildVmtNode(sT,0,sT.size(),vmap));

    // Initial
    VmtNode* newNode5 = new VmtNode("tmp5");
    string s5 = "(and (not " + sa + ") tmpI1)";
    newNode5->addChild(buildVmtNode(s5,0,s5.size(),vmap));
    
    VmtNode* iNode = new VmtNode("I");
    vmap.insert(Str2VmtNode("tmp5",newNode5));
    string sI = "(! tmp5 :init true)";
    iNode->addChild(buildVmtNode(sI,0,sI.size(),vmap));

    // Accepting
    VmtNode* newNode6 = new VmtNode("tmp6");
    string s6 = "(and " + sa + " tmpO2)";
    newNode6->addChild(buildVmtNode(s6,0,s6.size(),vmap));

    VmtNode* oNode = new VmtNode("O");
    vmap.insert(Str2VmtNode("tmp6",newNode6));
    string sO = "(! (not tmp6) :invar-property 0)";
    oNode->addChild(buildVmtNode(sO,0,sO.size(),vmap));

    // Merge List
    for (size_t i=0; i<_stateVarCnt; ++i) {
        _snList[0].push_back(xsList[1][i]);
        _snList[2].push_back(xsList[3][i]);
    }
    
    _snList[1] = a1->_snList[1];
    _snList[3] = a1->_snList[3];
    for (size_t i=0,size1=a2->_snList[1].size(); i<size1; ++i) {
        for (size_t j=0,size2=_snList[1].size(); j<size2; ++j)
            assert((_snList[1][j]->_num != a2->_snList[1][i]->_num));
        _snList[1].push_back(a2->_snList[1][i]);
    }
    for (size_t i=0,size1=a2->_snList[3].size(); i<size1; ++i) {
        for (size_t j=0,size2=_snList[3].size(); j<size2; ++j)
            assert((_snList[3][j]->_num != a2->_snList[3][i]->_num));
        _snList[3].push_back(a2->_snList[3][i]);
    }
    
    if (a1->_stateVarCnt > a2->_stateVarCnt) {
        _nxtList = a1->_nxtList;
        for (size_t i=0,size=a2->_nxtList.size(); i<size; ++i)
            if (a2->_nxtList[i]->_children[0]->_children[0]->_name[0] == 'n')
                _nxtList.push_back(a2->_nxtList[i]);
    }
    else {
        _nxtList = a2->_nxtList;
        for (size_t i=0,size=a1->_nxtList.size(); i<size; ++i)
            if (a1->_nxtList[i]->_children[0]->_children[0]->_name[0] == 'n')
                _nxtList.push_back(a1->_nxtList[i]);
    }
    string sNxt = "(! " + sa + " :next " + san + ")";
    VmtNode* newNodeSNxt = new VmtNode("tmp7");
    newNodeSNxt->addChild(buildVmtNode(sNxt,0,sNxt.size(),vmap));
    _nxtList.push_back(newNodeSNxt);

    _imdList = a1->_imdList;
    for (size_t i=0,size=a2->_imdList.size(); i<size; ++i) {
        _imdList.push_back(a2->_imdList[i]);
    }
    
    _imdList.push_back(newNode1);
    _imdList.push_back(newNode2);
    _imdList.push_back(newNode3);
    _imdList.push_back(newNode4);
    _imdList.push_back(newNode5);
    _imdList.push_back(newNode6);

    _itoList.push_back(iNode);
    _itoList.push_back(tNode);
    _itoList.push_back(oNode);

    renameDef();
}

VmtNode* Aut::getI()
{
    return _itoList[0]->_children[0]->_children[0];
}

VmtNode* Aut::getO()
{
    return _itoList[2]->_children[0]->_children[0]->_children[0];
}

VmtNode* Aut::getT()
{
    return _itoList[1]->_children[0]->_children[0];
}

void Aut::setI(VmtNode* n)
{
    _itoList[0]->_children[0]->_children[0] = n;
}

void Aut::setO(VmtNode* n)
{
    _itoList[2]->_children[0]->_children[0]->_children[0] = n;
}

void Aut::setT(VmtNode* n)
{
    _itoList[1]->_children[0]->_children[0] = n;
}

void Aut::renameDef()
{
    int dCnt = -1;
    for (size_t i = 0,size = _nxtList.size(); i < size; ++i) {
        _nxtList[i]->_name = "d"+itos(++dCnt);
    }
    for (size_t i = 0,size = _imdList.size(); i < size; ++i) {
        if (_imdList[i]->_name == "epsilon") continue;
        _imdList[i]->_name = "d"+itos(++dCnt);
    }
}

void Aut::renameStateVar(const size_t& delta)
{
    assert((_stateVarCnt+delta<=stateBitNum));
    _snList[0].clear();
    _snList[2].clear();
    for (size_t i = delta; i < _stateVarCnt+delta; ++i) {
        _snList[0].push_back(xsList[1][i]);
        _snList[2].push_back(xsList[3][i]);
    }
    ++gflag;
    for (size_t i=0,size=_nxtList.size(); i<size; ++i)
        _nxtList[i]->renameStateVar(delta);
    for (size_t i=0; i<3; ++i)
        _itoList[i]->renameStateVar(delta);
    for (size_t i=0,size=_imdList.size(); i<size; ++i)
        if (_imdList[i]->_flag != gflag) {
            cout << "renameStateVar::[WARNING] non-ito-fanin node=" << _imdList[i]->_name << endl;
            _imdList[i]->renameStateVar(delta);
        }
}

void Aut::vmtTokenize(const string& s,vector<string>& paramList, vector<string>& tokenList)
{
    size_t size = s.size();
    // Handle 
    //if (s[size-1] == 13) --size;
    size_t state = 0;
    for (size_t i = 0; i < size; ++i) {
        if (state == 0) {
            if (s[i] != ' ') continue;
            else {
                state = 1;
            }
        }
        else if (state == 1) {
            size_t j = i;
            while (s[j] != ' ') ++j;
            tokenList.push_back(s.substr(i,j-i));
            assert(s[++j] == '(');
            int dCnt = 1;
            while(dCnt != 0) {
                ++j;
                if (s[j] == '(') {
                    size_t k = ++j;
                    while (s[j] != ' ') ++j;
                    paramList.push_back(s.substr(k,j-k));
                    while (s[j] != ')') ++j;
                }
                else if (s[j] == ')') --dCnt;
            }
            j += 2;
            if (s[j] == 'I') tokenList.push_back("");
            else if (s[j] == 'B') {
                j += 4;
                if (s[j] == ')') tokenList.push_back("");
                else {
                    assert((s[j] == ' '));
                    size_t k = ++j;
                    if (s[j] == '(') {
                        dCnt = 1;
                        while(dCnt != 0) {
                            ++j;
                            if (s[j] == ')') --dCnt;
                            else if(s[j] == '(') ++dCnt;
                        }
                        tokenList.push_back(s.substr(k,j+1-k));
                    }
                    else {
                        while (s[j] != ')') ++j;
                        tokenList.push_back(s.substr(k,j-k));
                    }
                }
            }
            //cout << "vmtTokenize:: "<<tokenList[0] << "|" << tokenList[1] << "|" << endl;
            return;
        }
    }
}

VmtNode* Aut::buildVmtNode(const string& s,size_t bpos,size_t epos,Str2VmtNodeMap& vmap)
{
    //cout << s.substr(bpos,epos-bpos) << endl;
    if (s[bpos] != '(') {
        string name = s.substr(bpos,epos-bpos);
        Str2VmtNodeMap::iterator it = vmap.find(name);
        if (it != vmap.end()) return it->second;
        else {
            VmtNode* newNode = new VmtNode(name);
            //if (name != "!" && name != ":next" && name != "and" && name != "or" && name != "not" && name != ":init" && name != ":trans" && name != "invar-property")
                //vmap.insert(Str2VmtNode(name,newNode));
            return newNode;
        }
    }
    else {
        size_t i = bpos + 1;
        while (s[i] != ' ') ++i;
        string root = s.substr(bpos+1,i-(bpos+1));
        if (root != "and" && root != "or" && root != "not" && root != "!" && root != "=" && root != "+" && root != "-" && root != "div" && root != ">=" && root != "<=" && root != "<" && root != ">" && root != "str.len") {
            //cout << root << endl;
            Str2VmtNodeMap::iterator it = vmap.find(root);
            assert((it != vmap.end()));
            VmtNode* pNode = new VmtNode(root,PARAM);
            pNode->_source = it->second;
            while(s[i] != ')') {
                size_t j = ++i;
                while (s[i] != ' ' && s[i] != ')') ++i;
                string param = s.substr(j,i-j);
                Str2VmtNodeMap::iterator jt = vmap.find(param);
                switch (jt->second->_type) {
                    case INPUT   :
                        pNode->_paraList[0].insert(jt->second);
                        break;
                    case STATE   :
                        pNode->_paraList[1].insert(jt->second);
                        break;
                    case LEN     :
                        pNode->_paraList[2].insert(jt->second);
                        break;
                    case INPUT_N :
                        pNode->_paraList[3].insert(jt->second);
                        break;
                    case STATE_N :
                        pNode->_paraList[4].insert(jt->second);
                        break;
                    case LEN_N   :
                        pNode->_paraList[5].insert(jt->second);
                        break;
                    default      :
                        break;
                }
            }
            if (pNode->haveSameParam(it->second)) {
                //cout << "same" << endl;
                return it->second;
            }
            else {
                //cout << "not same" << endl;
                return pNode;
            }
        }
        VmtNode* newNode = new VmtNode(root);
        for (++i; i < epos; ++i) {
            size_t j = i;
            if (s[i] == '(') {
                int diff = 0;
                while (diff != 1) {
                    ++j;
                    if (s[j] == '(') --diff;
                    else if (s[j] == ')') ++diff;
                }
                newNode->addChild(buildVmtNode(s,i,j+1,vmap));
                i = j+1;
            }
            else {
                while (s[j] != ' ' && j != epos-1) ++j;
                newNode->addChild(buildVmtNode(s,i,j,vmap));
                i = j;
            }
        }
        return newNode;
    }
}

void Aut::buildParam()
{
    ++gflag;
    for (size_t i = 0; i < 3; ++i)
        _itoList[i]->buildParam();
    for (size_t i = 0,size = _imdList.size(); i < size; ++i)
        if (_imdList[i]->_flag != gflag) {
            cout << "buildParam::[WARNING] non-ito-fanin node=" << _imdList[i]->_name << endl;
            _imdList[i]->buildParam();
        }
}
