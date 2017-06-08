#include "autMgr.h"

extern AutMgr* autmgr;
static size_t& gflag = autmgr->getGFlag();

///////////////////////////////// Static Member ///////////////////////////////

size_t   Aut::inputBitNum = 8;
size_t   Aut::stateBitNum = 4;
size_t   Aut::lvarNum     = 4;
VarList  Aut::input       = Aut::initVarList(INPUT);
VarList  Aut::state       = Aut::initVarList(STATE);
VarList  Aut::lvar        = Aut::initVarList(LEN);
VmtNode* Aut::epsilon     = Aut::initEpsilon();

void Aut::test() {
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0, size = lvar[0].size(); j < size; ++j)
            cout << lvar[0][j]->_name << " " << lvar[0][j] << endl;
        cout << "expandCnt " << i << endl;
        expandVarList(LEN);
    }
}
VmtNode* Aut::initEpsilon()
{
    cout << "initEpsilon\n";
    VmtNode* n = new VmtNode("epsilon");
    n->addChild(new VmtNode("and"));
    for (size_t i = 0; i < inputBitNum; ++i) {
        VmtNode* notNode = new VmtNode("not");
        notNode->addChild(input[0][i]);
        n->_children[0]->addChild(notNode);
    }
    return n;
}

VarList Aut::initVarList(const VmtType& type) 
{
    cout << "initVarList\n";
    VarList v(2,VmtNodeList());
    VmtType nxtType;
    size_t numOfVars;
    string name;
    switch (type) {
        case INPUT :
            nxtType   = INPUT_N;
            numOfVars = inputBitNum;
            name      = "x";
            break;
        case STATE :
            nxtType   = STATE_N;
            numOfVars = stateBitNum;
            name      = "s";
            break;
        case LEN   :
            nxtType   = LEN_N;
            numOfVars = lvarNum;
            name      = "n";
            break;
        default :
            break;
    }
    for (size_t i = 0; i < numOfVars; ++i) {
        v[0].push_back(new VmtNode(name+itos(i),type,i));
        v[1].push_back(new VmtNode(name+itos(i)+".next",nxtType,i));
    }
    return v;
}

void Aut::expandVarList(const VmtType& type)
{
    if (type == STATE) {
        VmtNodeList v0 = state[0];
        VmtNodeList v1 = state[1];
        state[0].resize(stateBitNum * 2);
        state[1].resize(stateBitNum * 2);
        bool isConsistent = 1;
        for (size_t i = 0; i < stateBitNum; ++i) {
            if ( v0[i] != state[0][i] || v1[i] != state[1][i]) {
                isConsistent = 0;
                break;
            }
        }
        assert((isConsistent));
        for (size_t i = stateBitNum; i < stateBitNum * 2; ++i) {
            state[0][i] = new VmtNode( "s"+itos(i) , STATE , i );
            state[1][i] = new VmtNode( "s"+itos(i)+".next" , STATE_N , i );
        }
        stateBitNum *= 2;
    }
    else if (type == LEN) {
        VmtNodeList v0 = lvar[0];
        VmtNodeList v1 = lvar[1];
        lvar[0].resize(lvarNum * 2);
        lvar[1].resize(lvarNum * 2);
        bool isConsistent = 1;
        for (size_t i = 0; i < lvarNum; ++i) {
            if ( v0[i] != lvar[0][i] || v1[i] != lvar[1][i]) {
                isConsistent = 0;
                break;
            }
        }
        assert((isConsistent));
        for (size_t i = lvarNum; i < lvarNum * 2; ++i) {
            lvar[0][i] = new VmtNode("n"+itos(i),LEN,i);
            lvar[1][i] = new VmtNode("n"+itos(i)+".next",LEN_N,i);
        }
        lvarNum *= 2;
    }
    else {
        cout << "[ERROR::expandVarList] invalid type" << endl;
    }
}

void Aut::printVarList() 
{
    cout << "inputBitNum = " << inputBitNum << endl;
    for (size_t i = 0; i < inputBitNum; ++i) {
        input[0][i]->print(0);
        input[1][i]->print(0);
    }
    cout << "stateBitNum = " << stateBitNum << endl;
    for (size_t i = 0; i < stateBitNum; ++i) {
        state[0][i]->print(0);
        state[1][i]->print(0);
    }
    cout << "lvarNum = " << lvarNum << endl;
    for (size_t i = 0; i < lvarNum; ++i) {
        lvar[0][i]->print(0);
        lvar[1][i]->print(0);
    }
}

void Aut::printEpsilon()
{
    cout << "epsilon\n";
    epsilon->print(0);
}

void Aut::vmtTokenize(const string& s,vector<string>& paraList, vector<string>& tokenList)
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
                    paraList.push_back(s.substr(k,j-k));
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
            cout << "vmtTokenize:: "<<tokenList[0] << "|" << tokenList[1] << "|" << endl;
            for (size_t i = 0, s = paraList.size(); i < s; ++i)
                cout << paraList[i] << " ";
            cout << endl;
            return;
        }
    }
}

//TODO::refactor this function
VmtNode* Aut::buildVmtNode(const string& s, size_t bpos, size_t epos, Str2VmtNodeMap& vmap)
{
    //cout << s.substr(bpos,epos-bpos) << endl;
    if (s[bpos] != '(') {
        string name = s.substr(bpos,epos-bpos);
        Str2VmtNodeMap::iterator it = vmap.find(name);
        if (it != vmap.end()) return it->second;
        else {
            VmtNode* newNode = new VmtNode(name);
            return newNode;
        }
    }
    else {
        size_t i = bpos + 1;
        while (s[i] != ' ') ++i;
        string root = s.substr(bpos+1,i-(bpos+1));
        if ( !isReservedString(root) ) {
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

bool Aut::isReservedString(const string& s)
{
    if (s == "and" || s == "or" || s == "not" || s == "!"  || s == "="  || 
        s == "+"   || s == "-"  || s == "div" || s == ">=" || s == "<=" || 
        s == "<"   || s == ">"  || s == "str.len" || s == "str.indexof" || s == "str.substr")
        return 1;
    else
        return 0;
}

void Aut::check(Aut* a)
{
    assert( (a->_state[0].size() == a->_stateVarNum) );
    assert( (a->_state[1].size() == a->_stateVarNum) );
    assert( (a->_lvar[0].size() == a->_lvar[1].size()) );
    for (size_t i = 0, size = a->_lvar[0].size(); i < size; ++i)
        assert( (a->_lvar[0][i]->_idx == a->_lvar[1][i]->_idx) );
}

void Aut::check(Aut* a, const size_t& lvarIdx)
{
    check(a);
    for (size_t i = 0, size = a->_lvar[0].size(); i < size; ++i)
        assert( (lvarIdx != a->_lvar[0][i]->_idx) );
}

void Aut::check(Aut* a1, Aut* a2)
{
    check(a1);
    check(a2);
    for (size_t i = 0, size1 = a1->_lvar[0].size(); i < size1; ++i)
        for (size_t j = 0, size2 = a2->_lvar[0].size(); j < size2; ++j) {
            assert( (a1->_lvar[0][i]->_idx != a2->_lvar[0][j]->_idx) );
        }
}

void Aut::check(Aut* a1, Aut* a2, const size_t& lvarIdx)
{
    check(a1,a2);
    check(a1,lvarIdx);
    check(a2,lvarIdx);
}

///////////////////////////// Non-Static Member ///////////////////////////////

void Aut::init()
{
    _stateVarNum = 0;
    _state.assign(2,VmtNodeList());
    _lvar.assign(2,VmtNodeList());
    _predBV.assign(2,VmtNodeList());
    _predIV.assign(2,VmtNodeList());
    // insert epsilon and input to map
    _vmap.insert(Str2VmtNode("epsilon",epsilon));
    for (size_t i = 0; i < inputBitNum; ++i) {
        _vmap.insert(Str2VmtNode(input[0][i]->_name,input[0][i]));
        _vmap.insert(Str2VmtNode(input[1][i]->_name,input[1][i]));
    }
}

void Aut::clearParam()
{
    ++gflag;
    cout << "Aut::clearParam gflag=" << gflag << " " << &gflag << endl;
    for (size_t i = 0; i < 3; ++i)
        _itoList[i]->clearParam();
    for (size_t i = 0, size = _imdList.size(); i < size; ++i)
        if (_imdList[i]->_flag != gflag) {
            cout << "clearParam::[WARNING] non-ito-fanin node=" << _imdList[i]->_name << endl;
            _imdList[i]->clearParam();
        }
}

void Aut::buildParam()
{
    ++gflag;
    cout << "Aut::buildParam gflag=" << gflag << endl;
    for (size_t i = 0; i < 3; ++i)
        _itoList[i]->buildParam();
    for (size_t i = 0, size = _imdList.size(); i < size; ++i)
        if (_imdList[i]->_flag != gflag) {
            cout << "buildParam::[WARNING] non-ito-fanin node=" << _imdList[i]->_name << endl;
            _imdList[i]->buildParam();
        }
}

void Aut::renameDef()
{
    int dCnt = -1;
    for (size_t i = 0, size = _nxtList.size(); i < size; ++i) {
        _nxtList[i]->_name = "d" + itos(++dCnt);
    }
    for (size_t i = 0, size = _imdList.size(); i < size; ++i) {
        if (_imdList[i]->_name == "epsilon") continue;
        _imdList[i]->_name = "d" + itos(++dCnt);
    }
}

void Aut::shiftStateVar(const size_t& delta)
{
    while ( (_stateVarNum + delta) >= stateBitNum ) expandVarList(STATE);
    _state[0].clear();
    _state[1].clear();
    for (size_t i = delta; i < _stateVarNum + delta; ++i) {
        _state[0].push_back(state[0][i]);
        _state[1].push_back(state[1][i]);
    }
    ++gflag;
    for (size_t i = 0, size = _nxtList.size(); i < size; ++i)
        _nxtList[i]->shiftStateVar(delta);
    for (size_t i = 0; i < 3; ++i)
        _itoList[i]->shiftStateVar(delta);
    for (size_t i = 0, size = _imdList.size(); i < size; ++i)
        if (_imdList[i]->_flag != gflag) {
            cout << "shiftStateVar::[WARNING] non-ito-fanin node=" << _imdList[i]->_name << endl;
            _imdList[i]->shiftStateVar(delta);
        }
    check(this);
}

void Aut::defineFun(const string& fname, const string& body, VmtNodeList& list)
{
    cout << "defineFun " << fname << " " << body << endl;
    VmtNode* n = new VmtNode(fname);
    _vmap.insert(Str2VmtNode(fname,n));
    n->addChild(buildVmtNode(body,0,body.size(),_vmap));
    list.push_back(n);
}

void Aut::defineFun(const string& fname, const string& body, VmtNodeList& list, Str2VmtNodeMap& vmap)
{
    VmtNode* n = new VmtNode(fname);
    vmap.insert(Str2VmtNode(fname,n));
    n->addChild(buildVmtNode(body,0,body.size(),vmap));
    list.push_back(n);
}

void Aut::defineFun(const string& fname, const string& body, VmtNodeList& list, void (Aut::*set) (VmtNode*))
{
    VmtNode* n = new VmtNode(fname);
    _vmap.insert(Str2VmtNode(fname,n));
    n->addChild(buildVmtNode(body,0,body.size(),_vmap));
    list.push_back(n);
    (this->*set)(n);
}

void Aut::renameITO(const string& name, VmtNode* n)
{
    n->_name = name;
    _vmap.insert(Str2VmtNode(name, n));
}

void Aut::renameITOs2Aut(Aut* a1, Aut* a2)
{
    renameITO("tmpI1", a1->getI());
    renameITO("tmpI2", a2->getI());
    renameITO("tmpT1", a1->getT());
    renameITO("tmpT2", a2->getT());
    renameITO("tmpO1", a1->getO());
    renameITO("tmpO2", a2->getO());
}

void Aut::integrate(Aut* a1, Aut* a2)
{
    // Make sure _stateVarNum is already handled
    // _lvar may already be non-empty
    
    // state lvar into _vmap , merge _nxtList _imdList
    
    // State variable indexing is continuous
    for (size_t i = 0; i < _stateVarNum; ++i) {
        _state[0].push_back(state[0][i]);
        _state[1].push_back(state[1][i]);
        _vmap.insert(Str2VmtNode(state[0][i]->_name,state[0][i]));
        _vmap.insert(Str2VmtNode(state[1][i]->_name,state[1][i]));
    }
    
    // merge _lvar
    set<size_t> lvarIdx;

    for (size_t i = 0, size = a1->_lvar[0].size(); i < size; ++i)
        lvarIdx.insert(a1->_lvar[0][i]->_idx);
    for (size_t i = 0, size = a2->_lvar[0].size(); i < size; ++i)
        lvarIdx.insert(a2->_lvar[0][i]->_idx);
    for (size_t i = 0, size = _lvar[0].size(); i < size; ++i)
        lvarIdx.insert(_lvar[0][i]->_idx);
   
    _lvar[0].clear();
    _lvar[1].clear();

    for (set<size_t>::iterator it = lvarIdx.begin(); it != lvarIdx.end(); ++it) {
        _lvar[0].push_back(lvar[0][*it]);
        _lvar[1].push_back(lvar[1][*it]);
        _vmap.insert(Str2VmtNode(lvar[0][*it]->_name,lvar[0][*it]));
        _vmap.insert(Str2VmtNode(lvar[1][*it]->_name,lvar[1][*it]));
    }

    // build _nxtList
    size_t nxtCnt = 0;
    for (size_t i = 0; i < inputBitNum; ++i)
        defineFun("oldNxt" + itos(nxtCnt++), "(! " + input[0][i]->_name + " :next " + input[1][i]->_name + ")", _nxtList);

    for (size_t i = 0; i < _stateVarNum; ++i)
        defineFun("oldNxt" + itos(nxtCnt++), "(! " + state[0][i]->_name + " :next " + state[1][i]->_name + ")", _nxtList);

    for (size_t i = 0, size = _lvar[0].size(); i < size; ++i)
        defineFun("oldNxt" + itos(nxtCnt++), "(! " + _lvar[0][i]->_name + " :next " + _lvar[1][i]->_name + ")", _nxtList);
        
    // merge _imdList
    _imdList = a1->_imdList;
    for (size_t i = 0, size = a2->_imdList.size(); i < size; ++i)
        _imdList.push_back(a2->_imdList[i]);
}

size_t Aut::addStateVar(const size_t& num)
{
    // _state and _vmap is handled in integrate()
    size_t svbpos = _stateVarNum;
    for (size_t i = 0; i < num; ++i) {
        ++_stateVarNum;
        while (_stateVarNum >= stateBitNum) expandVarList(STATE);
    }
    return svbpos;
}

void Aut::addLVar(const size_t& lvarIdx)
{
    while (lvarIdx >= lvarNum) expandVarList(LEN);
    _lvar[0].push_back(lvar[0][lvarIdx]);
    _lvar[1].push_back(lvar[1][lvarIdx]);
    _vmap.insert(Str2VmtNode(lvar[0][lvarIdx]->_name,lvar[0][lvarIdx]));
    _vmap.insert(Str2VmtNode(lvar[1][lvarIdx]->_name,lvar[1][lvarIdx]));
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

/**Function*************************************************************

  Name        []

  Description [I/O]

***********************************************************************/
void Aut::print() const 
{
    for (size_t i = 0, s = _nxtList.size(); i < s; ++i)
        _nxtList[i]->print(0);
    for (size_t i = 0, s = _itoList.size(); i < s; ++i)
        _itoList[i]->print(0);
}

void Aut::parse(const char* fileName)
{
    string line;
    ifstream file(fileName);

    if (!file) {
        cout << "fail open file=" << fileName << endl;
        return;
    }
    
    vector<string> tokenList,paraList;
    int sCnt = 0;
    while (getline(file,line)) {
        if (line[0] == ';') {
            ++sCnt;
            continue;
        }
        tokenList.clear();
        paraList.clear();
        cout << "line=" << line << endl;
        vmtTokenize(line,paraList,tokenList);
        cout << "parse::       " << tokenList[0] << "|" << tokenList[1] << "|" <<endl;
        for (size_t i = 0, s = paraList.size(); i < s; ++i)
            cout << paraList[i] << " ";
        cout << endl;
        if (tokenList[0] == "epsilon") continue;
        if (sCnt == 0) {
            if (tokenList[0][0] == 's') {
                if (*(tokenList[0].rbegin()) != 't') {
                    ++_stateVarNum;
                    size_t idx = stoi(tokenList[0].substr(1));
                    cout << "s idx=" << idx << endl;
                    while (idx >= stateBitNum) expandVarList(STATE);
                    _vmap.insert(Str2VmtNode(state[0][idx]->_name,state[0][idx]));
                    _state[0].push_back(state[0][idx]);
                }
                else {
                    size_t idx = stoi(tokenList[0].substr(1,tokenList[0].find_last_of(".")-1));
                    cout << "s.next idx=" << idx << endl;
                    while (idx >= stateBitNum) expandVarList(STATE);
                    _vmap.insert(Str2VmtNode(state[1][idx]->_name,state[1][idx]));
                    _state[1].push_back(state[1][idx]);
                }
            }
            else if (tokenList[0][0] == 'n'){
                if (*(tokenList[0].rbegin()) != 't') {
                    size_t idx = stoi(tokenList[0].substr(1));
                    cout << "n idx=" << idx << endl;
                    while (idx >= lvarNum) expandVarList(LEN);
                    _vmap.insert(Str2VmtNode(lvar[0][idx]->_name,lvar[0][idx]));
                    _lvar[0].push_back(lvar[0][idx]);
                }
                else {
                    size_t idx = stoi(tokenList[0].substr(1,tokenList[0].find_last_of(".")-1));
                    cout << "n.next idx=" << idx << endl;
                    while (idx >= lvarNum) expandVarList(LEN);
                    _vmap.insert(Str2VmtNode(lvar[1][idx]->_name,lvar[1][idx]));
                    _lvar[1].push_back(lvar[1][idx]);
                }
            }
        }
        else {
            //cout << tokenList[0] << "|" << tokenList[1] << "|" <<endl;
            VmtNode* newNode1 = new VmtNode(tokenList[0]);
            VmtNode* newNode2 = buildVmtNode(tokenList[1],0,tokenList[1].size(),_vmap);
            newNode1->addChild(newNode2);
            for (size_t i = 0, s = paraList.size(); i < s; ++i) {
                Str2VmtNodeMap::iterator jt = _vmap.find(paraList[i]);
                assert((jt != _vmap.end()));
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
    print();
}

void Aut::write(const string& fileName)
{
    write(fileName.c_str());
}

void Aut::write(const char* fileName)
{
    ofstream file(fileName);
    cout << "gflag=" << gflag << endl;
    clearParam();
    cout << "gflag=" << gflag << endl;
    buildParam();
    ++gflag;

    for (size_t i = 0; i < inputBitNum; ++i)
        file << "(declare-fun " << input[0][i]->_name << " () Bool)\n";
    for (size_t i = 0; i < inputBitNum; ++i)
        file << "(declare-fun " << input[1][i]->_name << " () Bool)\n";

    for (size_t i = 0, size = _state[0].size(); i < size; ++i)
        file << "(declare-fun " << _state[0][i]->_name << " () Bool)\n";
    for (size_t i = 0, size = _state[1].size(); i < size; ++i)
        file << "(declare-fun " << _state[1][i]->_name << " () Bool)\n";

    for (size_t i = 0, size = _lvar[0].size(); i < size; ++i)
        file << "(declare-fun " << _lvar[0][i]->_name << " () Int)\n";
    for (size_t i = 0, size = _lvar[1].size(); i < size; ++i)
        file << "(declare-fun " << _lvar[1][i]->_name << " () Int)\n";
    
    for (size_t i = 0, size = _predBV[0].size(); i < size; ++i)
        file << "(declare-fun " << _predBV[0][i]->_name << " () Bool)\n";
    for (size_t i = 0, size = _predBV[1].size(); i < size; ++i)
        file << "(declare-fun " << _predBV[1][i]->_name << " () Bool\n";
    
    for (size_t i = 0, size = _predIV[0].size(); i < size; ++i)
        file << "(declare-fun " << _predIV[0][i]->_name << " () Int)\n";
    for (size_t i = 0, size = _predIV[1].size(); i < size; ++i)
        file << "(declare-fun " << _predIV[1][i]->_name << " () Int)\n";
    
    file << ";\n";
    
    for (size_t i = 0,size = _nxtList.size(); i < size; ++i) {
        _nxtList[i]->_flag = gflag;
        file << "(define-fun " << _nxtList[i]->_name << " () Bool ";
        _nxtList[i]->_children[0]->write(file);
        file << ")\n";
    }
    file << ";\n";
    
    file << "(define-fun epsilon ((x0 Bool) (x1 Bool) (x2 Bool) (x3 Bool) (x4 Bool) (x5 Bool) (x6 Bool) (x7 Bool)) Bool (and (not x0) (not x1) (not x2) (not x3) (not x4) (not x5) (not x6) (not x7)))\n";

    epsilon->_flag = gflag;

    for (size_t i = 0,size = _imdList.size(); i < size; ++i) {
        _imdList[i]->_flag = gflag;
        file << "(define-fun " << _imdList[i]->_name << " (";
        _imdList[i]->writeParam(file);
        file << ") Bool ";
        _imdList[i]->_children[0]->write(file);
        file << ")\n";
    }
    file << ";\n";

    for (size_t i = 0,size = _itoList.size(); i < size; ++i) {
        _itoList[i]->_flag = gflag;
        file << "(define-fun " << _itoList[i]->_name << " () Bool ";
        _itoList[i]->_children[0]->write(file);
        file << ")\n";
    }
    file.close();
}

/**Function*************************************************************

  Name        []

  Description [Operations]

***********************************************************************/

void Aut::addlen(const string& lvarIdxStr)
{
    cout << "addlen " << lvarIdxStr << endl;
    // Add Length Variable
    size_t lvarIdx = stoi(lvarIdxStr);
    check(this,lvarIdx);
    addLVar(lvarIdx);
    
    // Add Next State
    defineFun("tmpNxt", "(! " + lvar[0][lvarIdx]->_name + " :next " + lvar[1][lvarIdx]->_name + ")", _nxtList);

    // Add Length Constraint
    renameITO("tmpT", getT());
    defineFun("tmp1", "(and (not epsilon) (= " + lvar[1][lvarIdx]->_name + " (+ " + lvar[0][lvarIdx]->_name + " 1)))", _imdList);
    defineFun("tmp2", "(and epsilon (= " + lvar[1][lvarIdx]->_name + " " + lvar[0][lvarIdx]->_name + "))", _imdList);
    defineFun("tmp3", "(and tmpT (or tmp1 tmp2))", _imdList, &Aut::setT);
    //renameDef();
}

void Aut::intersect(Aut* a1, Aut* a2)
{
    check(a1,a2);

    // Shift State Variable of a2
    a2->shiftStateVar(a1->_stateVarNum);
    _stateVarNum = a1->_stateVarNum + a2->_stateVarNum;
    
    // Integrate _state, _lvar, _nxtList, _imdList into this
    integrate(a1,a2);
    
    // Rename ITO
    renameITOs2Aut(a1,a2);

    // Add Epsilon Loop
    string body = "(and epsilon";
    for (size_t i = 0; i < a1->_stateVarNum; ++i)
        body += " (= " + a1->_state[1][i]->_name + " " + a1->_state[0][i]->_name + ")";
    body += ")";
    defineFun("tmp1", body, _imdList);

    body = "(and epsilon";
    for (size_t i = 0; i < a2->_stateVarNum; ++i)
        body += " (= " + a2->_state[1][i]->_name + " " + a2->_state[0][i]->_name + ")";
    body += ")";
    defineFun("tmp2", body, _imdList);

    // Initial
    defineFun("tmp4", "(and tmpI1 tmpI2)", _imdList);
    defineFun("I", "(! tmp4 :init true)", _itoList);
    
    // Transition
    defineFun("tmp3", "(and (or tmpT1 tmp1) (or tmpT2 tmp2))", _imdList);
    defineFun("T", "(! tmp3 :trans true)", _itoList);

    // Accepting
    defineFun("tmp5", "(and tmpO1 tmpO2)", _imdList);
    defineFun("O", "(! (not tmp5) :invar-property 0)", _itoList);

    //renameDef();
}

void Aut::concate(Aut* a1,Aut* a2)
{
    check(a1,a2);
    
    _stateVarNum = (a1->_stateVarNum > a2->_stateVarNum)? a1->_stateVarNum : a2->_stateVarNum;
    size_t svbpos = addStateVar(1);
    string sa  = state[0][svbpos]->_name;
    string san = state[1][svbpos]->_name;
    
    // Integrate _state, _lvar, _nxtList, _imdList into this
    integrate(a1,a2);
    
    // Rename ITO
    renameITOs2Aut(a1,a2);

    // Initial
    defineFun("tmp5", "(and (not " + sa + ") tmpI1)",_imdList);
    defineFun("I", "(! tmp5 :init true)", _itoList);
    
    // Transition
    string body = "(and (not " + sa + ") (not " + san + ") tmpT1";
    for (size_t i = 0, size = a2->_lvar[0].size(); i < size; ++i) {
        body += " (= " + a2->_lvar[1][i]->_name + " " + a2->_lvar[0][i]->_name + ")";
    }
    body += ")"; 
    defineFun("tmp1", body, _imdList);

    body = "(and " + sa + " " + san + " tmpT2";
    for (size_t i = 0, size = a1->_lvar[0].size(); i < size; ++i) {
        body += " (= " + a1->_lvar[1][i]->_name + " " + a1->_lvar[0][i]->_name + ")";
    }
    body += ")"; 
    defineFun("tmp2", body, _imdList);

    VmtNode* newNodeI2 = new VmtNode("I2_PARAM",PARAM);
    newNodeI2->_source = a2->getI();
    _vmap.insert(Str2VmtNode("I2_PARAM",newNodeI2));
    for (size_t i = 0, size = a2->_state[1].size(); i < size; ++i)
        newNodeI2->_paraList[4].insert(a2->_state[1][i]);

    body = "(and epsilon (not " + sa + ") " + san + " tmpO1 I2_PARAM";
    for (size_t i = 0, size = a1->_lvar[0].size(); i < size; ++i) {
        body += " (= " + a1->_lvar[1][i]->_name + " " + a1->_lvar[0][i]->_name + ")";
    }
    for (size_t i = 0, size = a2->_lvar[1].size(); i < size; ++i) {
        body += " (= " + a2->_lvar[1][i]->_name + " " + a2->_lvar[0][i]->_name + ")";
    }
    body += ")"; 
    defineFun("tmp3", body, _imdList);

    defineFun("tmp4", "(or tmp1 tmp2 tmp3)", _imdList);
    defineFun("T", "(! tmp4 :trans true)", _itoList);

    // Accepting
    defineFun("tmp6", "(and " + sa + " tmpO2)", _imdList);
    defineFun("O", "(! (not tmp6) :invar-property 0)", _itoList);

    //renameDef();
}

void Aut::replace(Aut* a1, Aut* a2, Aut* a3, Aut* a4)
{
    renameDef();
}

void Aut::addpred(const string& fileName)
{
    string line;
    ifstream file(fileName.c_str());

    if (!file) {
        cout << "fail open file=" << fileName << endl;
        return;
    }
    size_t sCnt = 0, nxtCnt, pCnt = 0;
    Str2VmtNodeMap predVMap;
    while (getline(file,line)) {
        if (line == ";") {
            ++sCnt; continue;
        }
        if      (sCnt == 0) parseDef(line, nxtCnt, predVMap);
        else if (sCnt == 1) parsePred(line, pCnt, predVMap);
        else {
            cout << "[ERROR::Aut::addpred] invalid sCnt=" << sCnt << endl;
        }
    }
    
    // renaming
    for (size_t i = 0, size = _predBV[0].size(); i < size; ++i) {
        string rename = "B" + itos(i);
        _predBV[0][i]->_name = rename;
        _predBV[1][i]->_name = rename + ".next";
    }
    
    for (size_t i = 0, size = _predIV[0].size(); i < size; ++i) {
        string rename = "I" + itos(i);
        _predIV[0][i]->_name = rename;
        _predIV[1][i]->_name = rename + ".next";
    }

    VmtNode* oNode = getO();
    VmtNode* newNode1 = new VmtNode("tmp1");
    VmtNode* and1 = new VmtNode("and");
    and1->addChild(oNode);
    for (size_t i = 0, size = _predList.size(); i < size; ++i)
        and1->addChild(_predList[i]);
    newNode1->addChild(and1);
    _imdList.push_back(newNode1);
    setO(newNode1);
    
    VmtNode* iNode = getI();
    VmtNode* newNode2 = new VmtNode("tmp2");
    VmtNode* and2 = new VmtNode("and");
    and2->addChild(iNode);
    int eCnt = -1;
    for (size_t i = 0, size = _lvar[0].size(); i < size; ++i) {
        VmtNode* newNode3 = new VmtNode( "e" + itos(++eCnt) , NOPARAM );
        VmtNode* newNode4 = new VmtNode("=");
        newNode4->addChild(_lvar[0][i]);
        newNode4->addChild(new VmtNode("0"));
        newNode3->addChild(newNode4);
        _imdList.push_back(newNode3);
        and2->addChild(newNode3);
    }
    newNode2->addChild(and2);
    _imdList.push_back(newNode2);
    setI(newNode2);
}

void Aut::parseDef(const string& line, size_t& nxtCnt, Str2VmtNodeMap& vmap)
{
    cout << "line=" << line << endl;
    size_t i = 0, j = 0;
    string name, nameNxt, type;
    while (line[i] != ' ') ++i;
    j = ++i;
    while (line[i] != ' ') ++i;
    name = line.substr(j,i-j);
    nameNxt = name + ".next";
    i += 4;
    j = i;
    while (line[i] != ')') ++i;
    type = line.substr(j,i-j);
    cout << "name=" << name << " nameNxt=" << nameNxt << " type=" << type << endl;
    
    if (type == "Bool") {
        VmtNode* newNode0 = new VmtNode(name);
        VmtNode* newNode1 = new VmtNode(nameNxt);
        vmap.insert(Str2VmtNode(name,newNode0));
        vmap.insert(Str2VmtNode(nameNxt,newNode1));
        _predBV[0].push_back(newNode0);
        _predBV[1].push_back(newNode1);
        /*string s = "(! " + name + " :next " + nameNxt + ")";
        VmtNode* newNode3 = new VmtNode("tmpNxt"+itos(++tmpNxtCnt));
        newNode3->addChild(Aut::buildVmtNode(s,0,s.size(),_vmap));
        _nxtList.push_back(newNode3);*/
        defineFun("tmpNxt" + itos(nxtCnt++), "(! " + name + " :next " + nameNxt + ")", _nxtList, vmap);
    }
    else if(type == "Int") {
        if (*(line.rbegin()) == ')') {
            VmtNode* newNode0 = new VmtNode(name);
            VmtNode* newNode1 = new VmtNode(nameNxt);
            vmap.insert(Str2VmtNode(name,newNode0));
            vmap.insert(Str2VmtNode(nameNxt,newNode1));
            _predIV[0].push_back(newNode0);
            _predIV[1].push_back(newNode1);
            /*string s = "(! " + name + " :next " + nameNxt + ")";
            VmtNode* newNode3 = new VmtNode("tmpNxt"+itos(++tmpNxtCnt));
            newNode3->addChild(Aut::buildVmtNode(s,0,s.size(),_vmap));
            _nxtList.push_back(newNode3);*/
            defineFun("tmpNxt" + itos(nxtCnt++), "(! " + name + " :next " + nameNxt + ")", _nxtList, vmap);
        }
        else {
            VmtNode* newNode = new VmtNode(name);
            vmap.insert(Str2VmtNode(name,newNode));
        }
    }
    else cout << "[ERROR::Aut::parseDef] invalid type=" << type << endl;
}

void Aut::parsePred(const string& line, size_t& pCnt, Str2VmtNodeMap& vmap)
{
    //cout << "line=" << line << endl;
    if (line[8] != '(') {
        size_t i = 8;
        while (line[i] != ')') ++i;
        string var = line.substr(8,i-8);
        cout << "var=" << var << endl;
        Str2VmtNodeMap::iterator it = vmap.find(var);
        assert((it != vmap.end()));
        //pNode->addChild(it->second);
        //_predList.push_back(pNode);
        defineFun( "p" + itos(pCnt++), var, _predList, vmap);
    }
    else {
        if ( *(line.rbegin()) == ')') {
            string body = line.substr(8,line.size()-1-8);
            cout << "regular=" << body << endl;
            //pNode->addChild(Aut::buildVmtNode(s,0,s.size(),vmap));
            //_predList.push_back(pNode);
            defineFun( "p" + itos(pCnt++), body, _predList, vmap);
        }
        else {
            size_t i = line.size() - 1;
            while (line[i] != ' ') --i;
            string lvarIdx = line.substr(i+1);
            size_t j = i--;
            while (line[i] != ' ') --i;
            string type = line.substr(i+1,j-(i+1));
            
            //cout << "type=" << type << " lcvar=" << lcvar << endl;

            i = 8;
            size_t dCnt = 1;
            while (dCnt != 0) {
                ++i;
                if (line[i] == '(') ++dCnt;
                else if(line[i] == ')') --dCnt;
            }
            ++i;
            string body = line.substr(8,i-8);
            //cout << "strlen=" << s << endl;
            VmtNode* slNode = Aut::buildVmtNode(body,0,body.size(),vmap);
            
            if (type == "cstrlen") {
                VmtNode* pNode = new VmtNode( "p" + itos(pCnt++) );
                VmtNode* lvarNode = new VmtNode( "n" + lvarIdx );
                slNode->_children[1] = lvarNode;
                pNode->addChild(slNode);
                _predList.push_back(pNode);
            }
            else if (type == "vstrlen") {
                VmtNode* pNode = new VmtNode( "p" + itos(pCnt++) );
                VmtNode* lvarNode = new VmtNode( "n" + lvarIdx );
                slNode->_children[1] = lvarNode;
                pNode->addChild(slNode);
                _predList.push_back(pNode);
            }
            else {
                cout << "readPredFile::[WARNING] invalid type=" << type << endl;
            }
        }
    }
}
