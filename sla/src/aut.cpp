#include "autMgr.h"


extern AutMgr* autmgr;
static size_t& gflag = autmgr->getGFlag();

///////////////////////////////// Static Member ///////////////////////////////

size_t   Aut::inputBitNum = INPUT_ENCODE_BIT_NUM + 1;
size_t   Aut::stateBitNum = INIT_STATE_BIT_NUM;
size_t   Aut::lvarNum     = INIT_LVAR_NUM;
size_t   Aut::evarNum     = 0;
VarList  Aut::input       = Aut::initVarList(INPUT);
VarList  Aut::state       = Aut::initVarList(STATE);
VarList  Aut::lvar        = Aut::initVarList(LEN);
VarList  Aut::evar        = Aut::initVarList(EXIST);
VmtNode* Aut::epsilon     = Aut::initSpecialAlphabet(EPSILON);
VmtNode* Aut::leftAngle   = Aut::initSpecialAlphabet(LEFT_ANGLE);
VmtNode* Aut::rightAngle  = Aut::initSpecialAlphabet(RIGHT_ANGLE);


void Aut::test() {
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0, size = lvar[0].size(); j < size; ++j)
            cout << lvar[0][j]->_name << " " << lvar[0][j] << endl;
        cout << "expandCnt " << i << endl;
        expandVarList(LEN);
    }
}
VmtNode* Aut::initSpecialAlphabet(const AType& type)
{
    string name;
    size_t encode;
    switch (type) {
        case EPSILON     : name   = "epsilon";
                           encode = EPSILON_ENCODE;
                           break;
        case LEFT_ANGLE  : name   = "leftAngle";
                           encode = LEFT_ANGLE_BRACKET_ENCODE;
                           break;
        case RIGHT_ANGLE : name   = "rightAngle";
                           encode = RIGHT_ANGLE_BRACKET_ENCODE;
                           break;
        default          : break;
    }
    cout << "Aut::initSpecialAllphabet::" << name << endl;
    VmtNode* n = new VmtNode(name);
    n->addChild(new VmtNode("and"));
    string bitstr = Uint2BitString(encode,inputBitNum);
    for (size_t i = 0; i < inputBitNum; ++i) {
        if (bitstr[i] == '0') {
            VmtNode* notNode = new VmtNode("not");
            notNode->addChild(input[0][i]);
            n->_children[0]->addChild(notNode);
        }
        else {
            assert( (bitstr[i] == '1') );
            n->_children[0]->addChild(input[0][i]);
        }
        n->_paramList[0].insert(input[0][i]);
    }
    return n;
}

VarList Aut::initVarList(const VmtType& type) 
{
    assert( (type == INPUT || type == STATE || type == LEN || type == EXIST));
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
        case EXIST : 
            return v;
    }
    cout << "Aut::initVarList::" << name << endl;
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
    cout << "\nstateBitNum = " << stateBitNum << endl;
    for (size_t i = 0; i < stateBitNum; ++i) {
        state[0][i]->print(0);
        state[1][i]->print(0);
    }
    cout << "\nlvarNum = " << lvarNum << endl;
    for (size_t i = 0; i < lvarNum; ++i) {
        lvar[0][i]->print(0);
        lvar[1][i]->print(0);
    }
}

void Aut::printSpecialAlphabet(const AType& type)
{
    switch (type) {
        case EPSILON     : cout << "epsilon\n";
                           epsilon->print(0);
                           break;
        case LEFT_ANGLE  : cout << "leftAngle\n";
                           leftAngle->print(0);
                           break;
        case RIGHT_ANGLE : cout << "rightAngle\n";
                           rightAngle->print(0);
                           break;
        default          : break;
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
            #ifndef AUT_NDEBUG
                cout << "vmtTokenize:: "<<tokenList[0] << "|" << tokenList[1] << "|" << endl;
                for (size_t i = 0, s = paramList.size(); i < s; ++i)
                    cout << paramList[i] << " ";
                cout << endl;
            #endif
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
            VmtNode* pNode = new VmtNode(root,it->second);
            while(s[i] != ')') {
                size_t j = ++i;
                while (s[i] != ' ' && s[i] != ')') ++i;
                string param = s.substr(j,i-j);
                Str2VmtNodeMap::iterator jt = vmap.find(param);
                assert( (jt->second->_type >= 0 && jt->second->_type <= 5) );
                switch (jt->second->_type) {
                    case INPUT   :
                        pNode->_paramList[0].insert(jt->second);
                        break;
                    case STATE   :
                        pNode->_paramList[1].insert(jt->second);
                        break;
                    case LEN     :
                        pNode->_paramList[2].insert(jt->second);
                        break;
                    case INPUT_N :
                        pNode->_paramList[3].insert(jt->second);
                        break;
                    case STATE_N :
                        pNode->_paramList[4].insert(jt->second);
                        break;
                    case LEN_N   :
                        pNode->_paramList[5].insert(jt->second);
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
    _vmap.insert(Str2VmtNode("leftAngle",leftAngle));
    _vmap.insert(Str2VmtNode("rightAngle",rightAngle));
    for (size_t i = 0; i < inputBitNum; ++i) {
        _vmap.insert(Str2VmtNode(input[0][i]->_name,input[0][i]));
        _vmap.insert(Str2VmtNode(input[1][i]->_name,input[1][i]));
    }
}

void Aut::clearParam()
{
    ++gflag;
    //cout << "Aut::clearParam::gflag=" << gflag << " " << &gflag << endl;
    for (size_t i = 0; i < 3; ++i)
        _itoList[i]->clearParam();
    for (size_t i = 0, size = _imdList.size(); i < size; ++i)
        if (_imdList[i]->_flag != gflag) {
            cout << "clearParam::node=" << _imdList[i]->_name << " type=" << _imdList[i]->getTypeStr() << endl;
            _imdList[i]->clearParam();
        }
}

void Aut::buildParam()
{
    ++gflag;
    //cout << "Aut::buildParam::gflag=" << gflag << endl;
    for (size_t i = 0; i < 3; ++i)
        _itoList[i]->buildParam();
    for (size_t i = 0, size = _imdList.size(); i < size; ++i)
        if (_imdList[i]->_flag != gflag) {
            cout << "buildParam::node=" << _imdList[i]->_name << " type=" << _imdList[i]->getTypeStr() << endl;
            _imdList[i]->buildParam();
        }
}

void Aut::renameDef()
{
    int dCnt = -1;
    for (size_t i = 0, size = _imdList.size(); i < size; ++i) {
        if (_imdList[i]->_name == "epsilon") continue;
        _imdList[i]->_name = "d" + itos(++dCnt);
    }
}

void Aut::shiftStateVar(const size_t& delta)
{
    while ( (_stateVarNum + delta) > stateBitNum ) expandVarList(STATE);
    _state[0].clear();
    _state[1].clear();
    for (size_t i = delta; i < _stateVarNum + delta; ++i) {
        _state[0].push_back(state[0][i]);
        _state[1].push_back(state[1][i]);
    }
    ++gflag;
    for (size_t i = 0; i < 3; ++i)
        _itoList[i]->shiftStateVar(delta);
    for (size_t i = 0, size = _imdList.size(); i < size; ++i)
        assert( (_imdList[i]->_flag == gflag) );
    check(this);
}

void Aut::defineFun(const string& fname, const string& body, VmtNodeList& list)
{
    //cout << "defineFun " << fname << " " << body << endl;
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

void Aut::defineFun(const string& fname, const VmtType& type,const string& body, VmtNodeList& list, Str2VmtNodeMap& vmap)
{
    VmtNode* n = new VmtNode(fname,type);
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

void Aut::addParamNode(const string& name, VmtNode* source, const VmtNodeList& l)
{
    VmtNode* n = new VmtNode(name,source);
    _vmap.insert(Str2VmtNode(name,n));
    for (size_t i = 0, size = l.size(); i < size; ++i)
        n->_paramList[4].insert(l[i]);
}

void Aut::addParamNode(const string& name, VmtNode* source, Aut* a, const size_t& encode, const bool& isExistential)
{
    VmtNode* n = new VmtNode(name,source);
    _vmap.insert(Str2VmtNode(name,n));
    for (size_t i = 0, size = a->_state[0].size(); i < size; ++i) {
        n->_paramList[1].insert(a->_state[0][i]);
        n->_paramList[4].insert(a->_state[1][i]);
    }
    for (size_t i = 0, size = a->_lvar[0].size(); i < size; ++i) {
        n->_paramList[1].insert(a->_lvar[0][i]);
        n->_paramList[4].insert(a->_lvar[1][i]);
    }
    
    if (isExistential) {
        const size_t& evbpos = encode;
        for (size_t i = evbpos; i < evbpos + inputBitNum; ++i)
            n->_paramList[0].insert(evar[0][i]);
    }
    else {
        string bitstr = Uint2BitString(encode,inputBitNum);
        for (size_t i = 0; i < inputBitNum; ++i)
            if (bitstr[i] == '0') n->_paramList[0].insert(new VmtNode("false"));
            else                  n->_paramList[0].insert(new VmtNode("true"));
    }
}

void Aut::renameITO(const string& name, VmtNode* n)
{
    n->_name = name;
    _vmap.insert(Str2VmtNode(name, n));
}

void Aut::renameITO1Aut()
{
    renameITO("tmpI1", getI());
    renameITO("tmpT1", getT());
    renameITO("tmpO1", getO());
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
    
    // state lvar into _vmap , merge _imdList
    
    // State variable indexing is continuous
    _state[0].clear();
    _state[1].clear();
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

    // merge _imdList
    assert( (_imdList.empty()) );
    _imdList = a1->_imdList;
    for (size_t i = 0, size = a2->_imdList.size(); i < size; ++i)
        _imdList.push_back(a2->_imdList[i]);
    //TODO : merge _predBV _predIV _predList 
}

size_t Aut::addStateVar(const size_t& num)
{
    // _state and _vmap is handled in integrate()
    size_t svbpos = _stateVarNum;
    for (size_t i = 0; i < num; ++i) {
        ++_stateVarNum;
        while (_stateVarNum > stateBitNum) expandVarList(STATE);
        _state[0].push_back(state[0][_stateVarNum-1]);
        _state[1].push_back(state[1][_stateVarNum-1]);
        _vmap.insert(Str2VmtNode(state[0][_stateVarNum-1]->_name,state[0][_stateVarNum-1]));
        _vmap.insert(Str2VmtNode(state[1][_stateVarNum-1]->_name,state[1][_stateVarNum-1]));
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

size_t Aut::addEVar()
{
    size_t evbpos = evarNum;
    for (size_t i = evarNum; i < evarNum + inputBitNum; ++i) {
        string name = "y" + itos(i);
        string nameNxt = name + ".next";
        VmtNode* e0 = new VmtNode( name, INPUT, i);
        VmtNode* e1 = new VmtNode( nameNxt, INPUT_N, i);
        evar[0].push_back(e0);
        evar[1].push_back(e1);
        _evar[0].push_back(e0);
        _evar[1].push_back(e1);
        _vmap.insert(Str2VmtNode(name,e0));
        _vmap.insert(Str2VmtNode(nameNxt,e1));
    }
    evarNum += inputBitNum;
    return evbpos;
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
    
    vector<string> tokenList,paramList;
    int sCnt = 0;
    while (getline(file,line)) {
        if (line[0] == ';') {
            ++sCnt;
            continue;
        }
        if (sCnt == 1) continue;
        tokenList.clear();
        paramList.clear();
        vmtTokenize(line,paramList,tokenList);
        #ifndef AUT_NDEBUG
            cout << "line=" << line << endl;
            cout << "parse::       " << tokenList[0] << "|" << tokenList[1] << "|" <<endl;
            for (size_t i = 0, s = paramList.size(); i < s; ++i)
                cout << paramList[i] << " ";
            cout << endl;
        #endif
        if (tokenList[0] == "epsilon" || tokenList[0] == "leftAngle" || tokenList[0] == "rightAngle") continue;
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
            else if (tokenList[0][0] == 'y') {
                // Existential Variables
                if (*(tokenList[0].rbegin()) != 't') {
                    size_t idx = stoi(tokenList[0].substr(1));
                    assert( (idx < evarNum) );
                    _vmap.insert(Str2VmtNode(evar[0][idx]->_name,evar[0][idx]));
                    _evar[0].push_back(evar[0][idx]);
                }
                else {
                    size_t idx = stoi(tokenList[0].substr(1,tokenList[0].find_last_of(".")-1));
                    assert( (idx < evarNum) );
                    _vmap.insert(Str2VmtNode(evar[1][idx]->_name,evar[1][idx]));
                    _evar[1].push_back(evar[1][idx]);
                }
            }
        }
        else {
            //cout << tokenList[0] << "|" << tokenList[1] << "|" <<endl;
            VmtNode* newNode1 = new VmtNode(tokenList[0]);
            VmtNode* newNode2 = buildVmtNode(tokenList[1],0,tokenList[1].size(),_vmap);
            newNode1->addChild(newNode2);
            for (size_t i = 0, s = paramList.size(); i < s; ++i) {
                Str2VmtNodeMap::iterator jt = _vmap.find(paramList[i]);
                assert((jt != _vmap.end()));
                const VmtType& type = jt->second->_type;
                switch (type) {
                    case INPUT   :
                        newNode1->_paramList[0].insert(jt->second);
                        break;
                    case STATE   :
                        newNode1->_paramList[1].insert(jt->second);
                        break;
                    case LEN     :
                        newNode1->_paramList[2].insert(jt->second);
                        break;
                    case INPUT_N :
                        newNode1->_paramList[3].insert(jt->second);
                        break;
                    case STATE_N :
                        newNode1->_paramList[4].insert(jt->second);
                        break;
                    case LEN_N   :
                        newNode1->_paramList[5].insert(jt->second);
                        break;
                    default    :
                        break;
                }
            }
            //cout << "n1=" << newNode1->_name << " n2=" << newNode2->_name << endl;
             _vmap.insert(Str2VmtNode(tokenList[0],newNode1));
            if (sCnt == 2) _imdList.push_back(newNode1);
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
    //cout << "gflag=" << gflag << endl;
    clearParam();
    //cout << "gflag=" << gflag << endl;
    buildParam();

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
    
    for (size_t i = 0, size = _evar[0].size(); i < size; ++i)
        file << "(declare-fun " << _evar[0][i]->_name << " () Bool)\n";
    for (size_t i = 0, size = _evar[1].size(); i < size; ++i)
        file << "(declare-fun " << _evar[1][i]->_name << " () Bool\n";
    
    for (size_t i = 0, size = _predBV[0].size(); i < size; ++i)
        file << "(declare-fun " << _predBV[0][i]->_name << " () Bool)\n";
    for (size_t i = 0, size = _predBV[1].size(); i < size; ++i)
        file << "(declare-fun " << _predBV[1][i]->_name << " () Bool\n";
    
    for (size_t i = 0, size = _predIV[0].size(); i < size; ++i)
        file << "(declare-fun " << _predIV[0][i]->_name << " () Int)\n";
    for (size_t i = 0, size = _predIV[1].size(); i < size; ++i)
        file << "(declare-fun " << _predIV[1][i]->_name << " () Int)\n";
    
    file << ";\n";
    
    int nxtCnt = -1;
    for (size_t i = 0; i < inputBitNum; ++i)
        file << "(define-fun nxt" << ++nxtCnt << " () Bool " 
             << "(! " << input[0][i]->_name << " :next " << input[1][i]->_name<< "))\n";
    for (size_t i = 0, size = _state[0].size(); i < size; ++i)
        file << "(define-fun nxt" << ++nxtCnt << " () Bool " 
             << "(! " << _state[0][i]->_name << " :next " << _state[1][i]->_name<< "))\n";
    for (size_t i = 0, size = _lvar[0].size(); i < size; ++i)
        file << "(define-fun nxt" << ++nxtCnt << " () Bool " 
             << "(! " << _lvar[0][i]->_name << " :next " << _lvar[1][i]->_name<< "))\n";
    for (size_t i = 0, size = _evar[0].size(); i < size; ++i)
        file << "(define-fun nxt" << ++nxtCnt << " () Bool " 
             << "(! " << _evar[0][i]->_name << " :next " << _evar[1][i]->_name<< "))\n";
    for (size_t i = 0, size = _predBV[0].size(); i < size; ++i)
        file << "(define-fun nxt" << ++nxtCnt << " () Bool " 
             << "(! " << _predBV[0][i]->_name << " :next " << _predBV[1][i]->_name<< "))\n";
    for (size_t i = 0, size = _predIV[0].size(); i < size; ++i)
        file << "(define-fun nxt" << ++nxtCnt << " () Bool " 
             << "(! " << _predIV[0][i]->_name << " :next " << _predIV[1][i]->_name<< "))\n";
    
    file << ";\n";

    writeDefineFun(epsilon,file);
    writeDefineFun(leftAngle,file);
    writeDefineFun(rightAngle,file);

    for (size_t i = 0,size = _imdList.size(); i < size; ++i)
        writeDefineFun(_imdList[i],file);
    
    file << ";\n";

    for (size_t i = 0,size = _itoList.size(); i < size; ++i)
        writeDefineFun(_itoList[i],file);
    
    file.close();
}

void Aut::writeDefineFun(VmtNode* n, ofstream& file)
{
    n->_flag = gflag;
    file << "(define-fun " << n->_name << " (";
    n->writeParam(file);
    file << ") Bool ";
    n->_children[0]->write(file);
    file << ")\n";
}

/**Function*************************************************************

  Name        []

  Description [Operations]

***********************************************************************/

void Aut::addlen(const string& lvarIdxStr)
{
    // Add Length Variable
    size_t lvarIdx = stoi(lvarIdxStr);
    check(this,lvarIdx);
    addLVar(lvarIdx);
    string lv  = lvar[0][lvarIdx]->_name;
    string lvn = lvar[1][lvarIdx]->_name;

    // Add Length Constraint
    renameITO("tmpT", getT());
    defineFun("tmp1", "(and (not epsilon) (= " + lvn + " (+ " + lv + " 1)))", _imdList);
    defineFun("tmp2", "(and epsilon (= " + lvn + " " + lv + "))", _imdList);
    defineFun("tmp3", "(and tmpT (or tmp1 tmp2))", _imdList, &Aut::setT);
    //renameDef();
}

void Aut::intersect(Aut* a1, Aut* a2)
{
    check(a1,a2);

    // Shift State Variable of a2
    a2->shiftStateVar(a1->_stateVarNum);
    _stateVarNum = a1->_stateVarNum + a2->_stateVarNum;
    
    // Integrate _state, _lvar, _imdList into this
    integrate(a1,a2);
    
    // Rename ITO
    renameITOs2Aut(a1,a2);

    // Add Epsilon Loop
    defineFun("tmp1", "(and epsilon" + a1->CSNSEquiv(STATE) + ")", _imdList);
    defineFun("tmp2", "(and epsilon" + a2->CSNSEquiv(STATE) + ")", _imdList);

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
    
    // Integrate _state, _lvar, _imdList into this
    integrate(a1,a2);
    
    // Rename ITO
    renameITOs2Aut(a1,a2);

    // Initial
    defineFun("tmp5", "(and (not " + sa + ") tmpI1)",_imdList);
    defineFun("I", "(! tmp5 :init true)", _itoList);
    
    // Transition
    defineFun("tmp1", "(and (not " + sa + ") (not " + san + ") tmpT1" + a2->CSNSEquiv(LEN) + ")", _imdList);
    defineFun("tmp2", "(and " + sa + " " + san + " tmpT2" + a1->CSNSEquiv(LEN) + ")", _imdList);

    ParamSetList paramList (6,VmtNodeSet());
    for (size_t i = 0, size = a2->_state[1].size(); i < size; ++i)
        paramList[4].insert(a2->_state[1][i]);
    addParamNode("I2_PARAM",a2->getI(),a2->_state[1]);

    defineFun("tmp3", "(and epsilon (not " + sa + ") " + san + " tmpO1 I2_PARAM" + CSNSEquiv(LEN) + ")", _imdList);

    defineFun("tmp4", "(or tmp1 tmp2 tmp3)", _imdList);
    defineFun("T", "(! tmp4 :trans true)", _itoList);

    // Accepting
    defineFun("tmp6", "(and " + sa + " tmpO2)", _imdList);
    defineFun("O", "(! (not tmp6) :invar-property 0)", _itoList);

    //renameDef();
}

size_t Aut::mark()
{
    check(this);
    size_t svbpos = addStateVar(1);
    string sa  = state[0][svbpos]->_name;
    string san = state[1][svbpos]->_name;
    
    renameITO1Aut();
    
    // Initial State
    defineFun( "tmp1", "(and (not sa) tmpI1)", _imdList);
    defineFun( "I", "(! tmp1 :init true)", _itoList);

    // Transition Relation
    defineFun( "tmp2", "(and (= " + sa + " " + san + ") (not " + leftAngle->_name + ") (not " + rightAngle->_name + ") tmpT1)", _imdList);
    defineFun( "tmp3", "(or (and (not " + sa + ") " + san + " " + leftAngle->_name + ") (and " + sa + " (not " + san + ") " + rightAngle->_name + "))", _imdList);
    defineFun( "tmp4", "(or tmp2 (and" + CSNSEquiv(STATE) + CSNSEquiv(LEN) + " tmp3))", _imdList);
    defineFun( "T", "(! tmp4 :trans true)", _itoList);

    // Accepting States
    defineFun( "tmp5", "(and (not " + sa + ") tmpO1)", _imdList);
    defineFun( "O", "(! (not tmp5) :invar-property 0)", _itoList);

    renameDef();
    return svbpos;
}

void Aut::replace_A4(Aut* a1, Aut* a2)
{
    check(a1,a2);
    
    a2->shiftStateVar(a1->_stateVarNum);
    _stateVarNum = (a1->_stateVarNum > a2->_stateVarNum)? a1->_stateVarNum : a2->_stateVarNum;
    size_t svbpos = addStateVar(1);
    string sa  = state[0][svbpos]->_name;
    string san = state[1][svbpos]->_name;
    
    // Integrate _state, _lvar, _imdList into this
    integrate(a1,a2);
    
    // Rename ITO
    renameITOs2Aut(a1,a2);

    // Initial State
    defineFun( "tmp1", "(and (not " + sa + ") tmpT2)", _imdList);
    defineFun( "I", "(! tmp1 :init true)", _itoList);

    // Transition Relation
    defineFun( "tmp2", "(and (not " + sa + ") (not " + san + ") (not " + leftAngle->_name + ") (not" + rightAngle->_name + ") tmpT2)", _imdList);
    defineFun( "tmp3", "(and " + sa + " " + san + " (not " + leftAngle->_name + ") (not " + rightAngle->_name + ") tmpT1)", _imdList);
    defineFun( "tmp4", "(and (not " + sa + ") " + san + " " + leftAngle->_name + " tmpO2 tmpI1)", _imdList);
    defineFun( "tmp5", "(and " + sa + " (not " + san + ") " + rightAngle->_name + " tmpO1 tmpI2)", _imdList);
    defineFun( "tmp6", "(or tmp2 tmp3 tmp4 tmp5 tmp6)", _imdList);
    defineFun( "T", "(! tmp6 :trans true)", _itoList);
    
    // Accepting States
    defineFun( "tmp7", "(and (not " + sa + ") (or tmpO2 tmpI2))", _imdList);
    defineFun( "O", "(! (not tmp7) :invar-property 0)", _itoList);
    
    renameDef();
}

void Aut::replace(Aut* a1, Aut* a2, const size_t& alpha)
{
    check(a1,a2);
    
    a2->shiftStateVar(a1->_stateVarNum);
    _stateVarNum = a1->_stateVarNum + a2->_stateVarNum;
    size_t svbpos = addStateVar(1);
    string sa0  = state[0][alpha]->_name;
    string san0 = state[1][alpha]->_name;
    string sa1  = state[0][svbpos]->_name;
    string san1 = state[1][svbpos]->_name;
    
    // Integrate _state, _lvar, _imdList into this
    integrate(a1,a2);
    
    // Rename ITO
    renameITOs2Aut(a1,a2);

    // Initial State
    defineFun( "tmp1", "(and (not " + sa1 + ") tmpI1 tmpI2)", _imdList);
    defineFun( "I", "(! tmp1 :init true)", _itoList);
    
    // Transition Relation
    size_t evbpos = addEVar();
    addParamNode("I2_PARAM",a2->getI(),a2->_state[1]);

    addParamNode("T1_PARAM_1",a1->getT(),a1,LEFT_ANGLE_BRACKET_ENCODE,0);
    addParamNode("T1_PARAM_2",a1->getT(),a1,evbpos,1);
    addParamNode("T1_PARAM_3",a1->getT(),a1,RIGHT_ANGLE_BRACKET_ENCODE,0);

    
    defineFun( "tmp2", "(and (not " + sa0 + ") (not " + san0 + ") tmpT1 (not " + sa1 + ") (not " + san1 + ") tmpI2 I2_PARAM)", _imdList);
    defineFun( "tmp3", "(and (not " + sa0 + ") (not " + san0 + ") epsilon" + a1->CSNSEquiv(STATE) + a1->CSNSEquiv(LEN) + " (not " + sa1 + ") " + san1 + " tmpI2 I2_PARAM)", _imdList);
    defineFun( "tmp4", "(and (not " + sa0 + ") (not " + san0 + ")" + a1->CSNSEquiv(STATE) + a1->CSNSEquiv(LEN) + " " + sa1 + " " + san1 + " tmpT2)", _imdList);
    defineFun( "tmp5", "(and (not " + sa0 + ") " + san0 + " epsilon T1_PARAM_1 " + sa1 + " (not " + san1 + ") I2_PARAM tmpO2)", _imdList);
    defineFun( "tmp6", "(and " + sa0 + " " + san0 + " epsilon T1_PARAM_2 (not " + sa1 + ") (not " + san1 + ") tmpI2 I2_PARAM)", _imdList);
    defineFun( "tmp7", "(and " + sa0 + " (not " + san0 + ") epsilon T1_PARAM_3 (not " + sa1 + ") (not " + san1 + ") tmpI2 I2_PARAM)", _imdList);
    defineFun( "tmp8", "(or tmp2 tmp3 tmp4 tmp5 tmp6 tmp7)", _imdList);
    defineFun( "T", "(! tmp8 :trans true)", _itoList);
    
    // Accepting States
    defineFun( "tmp9", "(and (not " + sa1 + ") tmpO1 tmpO2)", _imdList);
    defineFun( "O", "(! (not tmp9) :invar-property 0)", _itoList);
    
    renameDef();
}

void Aut::prefix(const string& lvarIdxStr)
{
    // Add Length Variable
    size_t lvarIdx = stoi(lvarIdxStr);
    check(this,lvarIdx);
    addLVar(lvarIdx);
    size_t svbpos = addStateVar(1);
    string sa  = state[0][svbpos]->_name;
    string san = state[1][svbpos]->_name;
    string lv  = lvar[0][lvarIdx]->_name;
    string lvn = lvar[1][lvarIdx]->_name;
    
    renameITO1Aut();

    // Initial State
    defineFun( "tmp1", "(and (not " + sa + ") tmpI1)", _imdList);
    defineFun( "I", "(! tmp1 :init true)", _itoList);

    // Transition Relation
    size_t evbpos = addEVar();
    addParamNode("T1_PARAM",getT(),this,evbpos,1);

    defineFun( "T1_epsilon", "(or tmpT1 (and epsilon" + CSNSEquiv(STATE) + "))", _imdList);
    defineFun( "tmp2", "(or (and (not epsilon) (= " + lvn + " (+ " + lv + " 1))) (epsilon (= " + lvn + " " + lv + ")))", _imdList);
    defineFun( "tmp3", "(and (not " + sa + ") (not " + san + ") T1_epsilon tmp2)", _imdList);
    defineFun( "tmp4", "(and (not " + sa + ") " + san + " epsilon" + CSNSEquiv(STATE) + CSNSEquiv(LEN) + " (= " + lvn + " " + lv + "))", _imdList);
    defineFun( "tmp5", "(and " + sa + " " + san + " epsilon T1_PARAM (= " + lvn + " " + lv + "))", _imdList);
    defineFun( "tmp6", "(or tmp3 tmp4 tmp5)", _imdList);
    defineFun( "T", "(! tmp6 :trans true)", _itoList);
    
    // Accepting States
    defineFun( "tmp7", "(and " + sa + " tmpO1)", _imdList);
    defineFun( "O", "(! (not tmp7) :invar-property 0)", _itoList);
    renameDef();
}

void Aut::suffix(const string& lvarIdxStr)
{
    // Add Length Variable
    size_t lvarIdx = stoi(lvarIdxStr);
    check(this,lvarIdx);
    addLVar(lvarIdx);
    size_t svbpos = addStateVar(1);
    string sa  = state[0][svbpos]->_name;
    string san = state[1][svbpos]->_name;
    string lv  = lvar[0][lvarIdx]->_name;
    string lvn = lvar[1][lvarIdx]->_name;

    // Initial State
    defineFun( "tmp1", "(and (not" + sa + ") tmpI1)", _imdList);
    defineFun( "I", "(! tmp1 :init true)", _itoList);
    
    // Transition Relation
    size_t evbpos = addEVar();
    VmtNode* n = new VmtNode("yEpsilon");
    n->addChild(new VmtNode("and"));
    for (size_t i = evbpos; i < evbpos + inputBitNum; ++i) {
        VmtNode* notNode = new VmtNode("not");
        notNode->addChild(evar[0][i]);
        n->_children[0]->addChild(notNode);
    }
    _vmap.insert(Str2VmtNode("yEpsilon",n));

    defineFun( "T1_epsilon", "(or tmpT1 (and epsilon" + CSNSEquiv(STATE) + "))", _imdList);
    addParamNode("T1_PARAM",_imdList.back(),this,evbpos,1);

    defineFun( "tmp2", "(and T1_PARAM (or (and (not yEpsilon) (= " + lvn + " (+ " + lv + " 1))) (and yEpsilon (= " + lvn + " " + lv + "))))", _imdList);
    defineFun( "tmp3", "(and (not " + sa + ") (not " + san + ") epsilon tmp2)", _imdList);
    defineFun( "tmp4", "(and (not " + sa + ") " + san + " epsilon" + CSNSEquiv(STATE) + CSNSEquiv(LEN) + " (= " + lvn + " " + lv + "))", _imdList);
    defineFun( "tmp5", "(and " + sa + " " + san + " tmpT1 (= " + lvn + " " + lv + "))", _imdList);
    defineFun( "tmp6", "(or tmp3 tmp4 tmp5)", _imdList);
    defineFun( "T", "(! tmp6 :trans true)", _itoList);
    
    // Accepting States
    defineFun( "tmp7", "(and " + sa + " tmpO1)", _imdList);
    defineFun( "O", "(! (not tmp7) :invar-property 0)", _itoList);
    
    renameDef();
}

void Aut::substr(const string& n1, const string& n2)
{
    prefix(n2);
    suffix(n1);
}

void Aut::addpred(const string& fileName)
{
    string line;
    ifstream file(fileName.c_str());

    if (!file) {
        cout << "fail open file=" << fileName << endl;
        return;
    }
    size_t sCnt = 0, pCnt = 0;
    Str2VmtNodeMap predVMap;
    while (getline(file,line)) {
        if (line == ";") {
            ++sCnt; continue;
        }
        if      (sCnt == 0) parseDef(line, predVMap);
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
    for (size_t i = 0, size = _predList.size(); i < size; ++i) {
        _imdList.push_back(_predList[i]);
        and1->addChild(_predList[i]);
    }
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

void Aut::parseDef(const string& line, Str2VmtNodeMap& vmap)
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
    
    VmtNode* newNode0 = new VmtNode(name);
    VmtNode* newNode1 = new VmtNode(nameNxt);
    vmap.insert(Str2VmtNode(name,newNode0));
    vmap.insert(Str2VmtNode(nameNxt,newNode1));

    if (type == "Bool") {
        _predBV[0].push_back(newNode0);
        _predBV[1].push_back(newNode1);
    }
    else if(type == "Int") {
        _predIV[0].push_back(newNode0);
        _predIV[1].push_back(newNode1);
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
        defineFun( "p" + itos(pCnt++), NOPARAM, var, _predList, vmap);
    }
    else {
        if ( *(line.rbegin()) == ')') {
            string body = line.substr(8,line.size()-1-8);
            cout << "regular=" << body << endl;
            defineFun( "p" + itos(pCnt++), NOPARAM, body, _predList, vmap);
        }
        else {
            size_t i = 8;
            size_t dCnt = 1;
            while (dCnt != 0) {
                ++i;
                if (line[i] == '(') ++dCnt;
                else if(line[i] == ')') --dCnt;
            }
            string body = line.substr(8,++i-8);
            VmtNode* root = buildVmtNode(body,0,body.size(),vmap);

            vector<string> tokenList;
            str2tokens(line.substr(line.find_last_of(';')+1),tokenList);
            
            if (tokenList[0] == "len") {
                assert( (tokenList.size() == 2) );
                assert( (root->_name == "=") );
                assert( (root->_children[0]->_name == "str.len" || root->_children[1]->_name == "str.len") );
                string body = "(= ";
                size_t lvarIdx = stoi(tokenList[1]);
                if (root->_children[0]->_name == "str.len") 
                    body += root->_children[1]->_name + " " + lvar[0][lvarIdx]->_name + ")";
                else
                    body += root->_children[0]->_name + " " + lvar[0][lvarIdx]->_name + ")";
                defineFun( "p" + itos(pCnt++), NOPARAM, body, _predList, vmap);
            }
            else if (tokenList[0] == "substr") {
                assert( (tokenList.size() == 3) );
                size_t n0 = stoi(tokenList[1]);
                size_t n1 = stoi(tokenList[2]);
                string body0 = "(= " + lvar[0][n0]->_name + " (+ " + root->_children[1]->_name + " 1))";
                string body1 = "(= " + lvar[0][n1]->_name + " (+ " + root->_children[1]->_name + " " + root->_children[2]->_name + "))";
                defineFun( "p" + itos(pCnt++), NOPARAM, body0, _predList, vmap);
                defineFun( "p" + itos(pCnt++), NOPARAM, body1, _predList, vmap);
            }
            else if (tokenList[0] == "indexof") {
                size_t n0 = stoi(tokenList[1]);
                size_t n1 = stoi(tokenList[2]);
                string body0 = "(= " + root->_children[0]->_name + " (+ " + lvar[0][n0]->_name + " " + lvar[0][n1]->_name + "))";
                string body1 = "(= " + root->_children[1]->_children[2]->_name + " " + lvar[0][n1]->_name + ")";
                defineFun( "p" + itos(pCnt++), NOPARAM, body0, _predList, vmap);
                defineFun( "p" + itos(pCnt++), NOPARAM, body1, _predList, vmap);
            }
            else {
                cout << "readPredFile::[WARNING] invalid type=" << tokenList[0] << endl;
            }
        }
    }
}

string Aut::CSNSEquiv(const VmtType& type)
{
    assert( (type == STATE || type == LEN) );
    string ret;
    if (type == STATE) {
        for (size_t i = 0; i < _stateVarNum; ++i)
            ret += " (=" + _state[0][i]->_name + " " + _state[1][i]->_name + ")";
    }
    else {
        for (size_t i = 0, size = _lvar[0].size(); i < size; ++i)
            ret += " (=" + _lvar[0][i]->_name + " " + _lvar[1][i]->_name + ")";
    }
    return ret;
}
