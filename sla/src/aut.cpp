#include "autMgr.h"


extern AutMgr* autmgr;
static size_t& gflag = autmgr->getGFlag();

///////////////////////////////// Static Member ///////////////////////////////

size_t   Aut::inputBitNum = INPUT_ENCODE_BIT_NUM + 1;
//size_t   Aut::stateBitNum = INIT_PI_BIT_NUM;
//size_t   Aut::lvarNum     = INIT_PI_BIT_NUM;
//size_t   Aut::evarNum     = 0;
//size_t   Aut::predBVNum   = 0;
//size_t   Aut::predIVNum   = 0;
//VarList  Aut::input       = Aut::initVarList(INPUT);
//VarList  Aut::evar        = Aut::initVarList(EXIST);
//VarList  Aut::state       = Aut::initVarList(STATE);
//VarList  Aut::lvar        = Aut::initVarList(LEN);
//VarList  Aut::predBV      = Aut::initVarList(PREDBV);
//VarList  Aut::predIV      = Aut::initVarList(PREDIV);
VarList  Aut::piList      = Aut::initPIList();
VmtNode* Aut::const0      = Aut::initConst(CONST0);
VmtNode* Aut::const1      = Aut::initConst(CONST1);
VmtNode* Aut::epsilon     = Aut::initSpecialAlphabet(EPSILON);
VmtNode* Aut::leftAngle   = Aut::initSpecialAlphabet(LEFT_ANGLE);
VmtNode* Aut::rightAngle  = Aut::initSpecialAlphabet(RIGHT_ANGLE);


void Aut::test() {
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0, size = lvar[0].size(); j < size; ++j)
            cout << lvar[0][j]->_name << " " << lvar[0][j] << endl;
        cout << "expandCnt " << i << endl;
        expandPIList(LEN);
    }
}

VmtNode* Aut::initConst(const VmtType& type)
{
    assert( (type == CONST0 || type == CONST1) );
    if (type == CONST0) return new VmtNode("false");
    else                return new VmtNode("true" );
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
    #ifndef AUT_NDEBUG
        cout << "Aut::initSpecialAllphabet::" << name << endl;
    #endif
    VmtNode* n = new VmtNode(name);
    n->addChild(new VmtNode("and"));

    // special alphabets are encoded with x0
    VmtType inputType        = INPUT;
    const VmtNodeList& input = piList[inputType];

    n->_children[0]->addChild(input[0]);
    n->_paramList[inputType].push_back(input[0]);

    string bitstr = Uint2BitString(encode,INPUT_ENCODE_BIT_NUM);
    for (size_t i = 0; i < INPUT_ENCODE_BIT_NUM; ++i) {
        if (bitstr[i] == '0') {
            VmtNode* notNode = new VmtNode("not");
            notNode->addChild(input[i+1]);
            n->_children[0]->addChild(notNode);
        }
        else {
            assert( (bitstr[i] == '1') );
            n->_children[0]->addChild(input[i+1]);
        }
        n->_paramList[inputType].push_back(input[i+1]);
    }
    return n;
}

VarList Aut::initPIList() 
{
    #ifndef AUT_NDEBUG
        cout << "Aut::initVarList::" << name << endl;
    #endif
    VarList v(PI_NUM,VmtNodeList());
    VmtType type = INPUT;
    for (size_t i = 0; i < inputBitNum; ++i)
        v[type].push_back( new VmtNode( getPISymbol(type) + itos(i) , i ) );
    return v;
}

//TODO
void Aut::printStaticDataMember() 
{
    for (size_t i = 0; i < PI_NUM; ++i) {
        cout << "[Aut::printStaticDataMember] " << getTypeStr(i) << endl;
        for (size_t j = 0, size = piList[i].size(); j < size; ++j) 
            piList[i][j]->print(0);
    }
    cout << "[Aut::printStaticDataMember] const0\n";
    const0->print(0);
    cout << "[Aut::printStaticDataMember] const1\n";
    const1->print(0);
    cout << "[Aut::printStaticDataMember] epsilon\n";
    epsilon->print(0);
    cout << "[Aut::printStaticDataMember] leftAngle\n";
    leftAngle->print(0);
    cout << "[Aut::printStaticDataMember] rightAngle\n";
    rightAngle->print(0);
}

bool Aut::isPI(const VmtType& type)
{
    return (type >= 0 && type < PI_NUM);
}

bool Aut::isCurPI(const VmtType& type)
{
    return (isPI(type) && type != STATE_N && type != LEN_N);
}

bool Aut::isLEAF(const VmtType& type)
{
    return (type < 30);
}

bool Aut::isIMD(const VmtType& type)
{
    return (type >= 30);
}

string Aut::getPISymbol(const size_t& idx)
{
    return getPISymbol(static_cast<VmtType>(idx));
}

string Aut::getPISymbol(const VmtType& type)
{
    assert( (isPI(type)) );
    string name;
    switch (type) {
        case INPUT   : name = "x"; break;
        case EXIST   : name = "y"; break;
        case STATE   : name = "s"; break;
        case LEN     : name = "n"; break;
        case STATE_N : name = "s"; break;
        case LEN_N   : name = "n"; break;
        case PREDBV  : name = "B"; break;
        case PREDIV  : name = "I"; break;
    }
    return name;
}

VmtType Aut::getPITypeByName(const string& name) {
    if (name[0] == 'x' )            return INPUT;
    else if (name[0] == 'y' )       return EXIST;
    else if (name[0] == 'B' )       return PREDBV;
    else if (name[0] == 'I' )       return PREDIV;
    else if (name[0] == 's' ) {
        if ( *(name.end()) == 't' ) return STATE_N;
        else                        return STATE;
    }
    else {
        assert( (name[0] == 'n') );
        if ( *(name.end()) == 't' ) return LEN_N;
        else                        return LEN;
    }
}

string Aut::getTypeStr(const size_t& idx) 
{
    return getTypeStr(static_cast<VmtType>(idx));
}


//TODO
string Aut::getTypeStr(const VmtType& type) 
{
    string typeStr;
    switch (type) {
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
        case PREDBV  : typeStr = "PREDBV";
                       break;
        case PREDIV  : typeStr = "PREDIV";
                       break;
        case CONST0  : typeStr = "CONST0";
                       break;
        case CONST1  : typeStr = "CONST1";
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

void Aut::expandPIList(const VmtType& type)
{
    assert( (isCurPI(type) && type != INPUT) );
    VarList v    = piList[type];
    string  name = getPISymbol(type);
    size_t  idx  = piList[type].size()
    if (type == STATE) {
        piList[type].push_back(new VmtNode( "s" + itos(idx)           , idx ));
        piList[type].push_back(new VmtNode( "s" + itos(idx) + ".next" , idx ));
    }
    else if (type == LEN) {
        piList[type].push_back(new VmtNode( "n" + itos(idx)           , idx ));
        piList[type].push_back(new VmtNode( "n" + itos(idx) + ".next" , idx ));
    }
    else {
        piList[type].push_back(new VmtNode( name + itos(idx) , idx ));
    }
}

void Aut::vmtTokenize(const string& s,vector<string>& paramList, vector<string>& tokenList)
{
    size_t size = s.size();
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
            /*
            #ifndef AUT_NDEBUG
                cout << "[Aut::vmtTokenize] "<<tokenList[0] << "|" << tokenList[1] << "|" << endl;
                for (size_t i = 0, s = paramList.size(); i < s; ++i)
                    cout << paramList[i] << " ";
                cout << endl;
            #endif
            */
            return;
        }
    }
}

//TODO::refactor this function
VmtNode* Aut::buildVmtNode(const string& s, size_t bpos, size_t epos, Str2VmtNodeMap& vmap)
{
    #ifndef AUT_NDEBUG
        cout << "[Aut::buildVmtNode] line = " << s.substr(bpos,epos-bpos) << endl;
    #endif
    if (s[bpos] != '(') {
        string name = s.substr(bpos,epos-bpos);
        Str2VmtNodeMap::iterator it = vmap.find(name);
        if (it != vmap.end()) return it->second;
        else                  return new VmtNode(name);
    }
    else {
        size_t i = bpos + 1;
        while (s[i] != ' ') ++i;
        string root = s.substr(bpos+1,i-(bpos+1));
        #ifndef AUT_NDEBUG
            cout << "[Aut::buildVmtNode] root = " << root << endl;
        #endif
        if ( !isReservedString(root) ) {
            Str2VmtNodeMap::iterator it = vmap.find(root);
            assert((it != vmap.end())); // define-fun in topological order
            VmtNode* source = it->second;
            VmtNode* pNode  = new VmtNode(root,source);
            while(s[i] != ')') {
                size_t j = ++i;
                while (s[i] != ' ' && s[i] != ')') ++i;
                string param = s.substr(j,i-j);
                Str2VmtNodeMap::iterator jt = vmap.find(param);
                assert( (jt != vmap.end()) );
                VmtType type = jt->second->_type;
                assert( (isPI(type) || type == CONST0 || type == CONST1) );
                if  (type == CONST0 || type == CONST1) type = INPUT;
                pNode->_paramList[type].push_back(jt->second);
            }
            if (pNode->haveSameParam(source)) {
                //cout << "[Aut::buildVmtNode] " << root << " same" << endl;
                return source;
            }
            else {
                cout << "[Aut::buildVmtNode] " << root << " PARAM" << endl;
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

// not include div
bool Aut::isReservedString(const string& s)
{
    if (s == "and" || s == "or" || s == "not" || s == "!"  || s == "="  || 
        s == "+"   || s == "-"  || s == ">="  || s == "<=" || s == "<"  || s == ">" ||
        s == "str.len" || s == "str.indexof" || s == "str.substr")
        return 1;
    else
        return 0;
}

void Aut::check(Aut* a)
{
    VmtType type0 = STATE;
    VmtType type1 = STATE_N;
    VmtType type2 = LEN;
    VmtType type3 = LEN_N;
    assert( (a->_piList[type0].size() == a->_stateVarNum) );
    assert( (a->_piList[type1].size() == a->_stateVarNum) );
    assert( (a->_piList[type2].size() == a->_piList[type3].size()) );
    for (size_t i = 0, size = a->_piList[type2].size(); i < size; ++i)
        assert( (a->_piList[type2][i]->_idx == a->_piList[type3][i]->_idx) );
}

void Aut::check(Aut* a, const size_t& lvarIdx)
{
    check(a);
    VmtType type = LEN;
    for (size_t i = 0, size = a->_piList[type].size(); i < size; ++i)
        assert( (lvarIdx != a->_piList[type][i]->_idx) );
}

void Aut::check(Aut* a1, Aut* a2)
{
    check(a1);
    check(a2);
    VmtType type = LEN;
    for (size_t i = 0, size1 = a1->_piList[type].size(); i < size1; ++i)
        for (size_t j = 0, size2 = a2->_piList[type].size(); j < size2; ++j) {
            assert( (a1->_piList[type][i]->_idx != a2->_piList[type][j]->_idx) );
        }
}

void Aut::check(Aut* a1, Aut* a2, const size_t& lvarIdx)
{
    check(a1,a2);
    check(a1,lvarIdx);
    check(a2,lvarIdx);
}

///////////////////////////// Non-Static Member ///////////////////////////////

void Aut::init(const string& fileName)
{
    size_t bpos = fileName.find_last_of('/') + 1;
    size_t epos = fileName.find_last_of('.');
    _name = fileName.substr(bpos,epos-bpos);
    _stateVarNum = 0;
    _piList.assign(PI_NUM,VmtNodeList());
    initVMap();
}

void Aut::initVMap()
{
    _vmap.clear();
    _vmap.insert(Str2VmtNode("false"     ,const0    ));
    _vmap.insert(Str2VmtNode("true"      ,const1    ));
    _vmap.insert(Str2VmtNode("epsilon"   ,epsilon   ));
    _vmap.insert(Str2VmtNode("leftAngle" ,leftAngle ));
    _vmap.insert(Str2VmtNode("rightAngle",rightAngle));
}

void Aut::buildVMap(const VmtType& type)
{
    assert( (isPI(type)) );
    for (size_t i = 0, size = _piList[type].size(); i < size; ++i)
        _vmap.insert(Str2VmtNode(_piList[type][i]->_name,_piList[type]));
}

void Aut::buildVMap(const VmtNodeList& moduleList)
{
    for (size_t i = 0, size = moduleList.size(); i < size; ++i) {
        assert( (moduleList[i]->_type == MODULE) );
        _vmap.insert(Str2VmtNode(moduleList[i]->_name,moduleList[i]));
    }
}

void Aut::assignGlobalPIList(const VmtType& type)
{
    assert( (type == EXIST || type == PREDBV || type == PREDIV) );
    VarList& global =  piList[type];
    VarList& local  = _piList[type];
    const size_t gsize = global.size();
    const size_t lsize = local.size();
    global.resize( gsize + lsize );
    for (size_t i = 0; i < lsize; ++i) {
        // rename and assign
        size_t idx = i + gsize;
        local[i]->_idx  = idx;
        local[i]->_name = getPISymbol(type) + itos(idx);
        global[idx]     = local[i];
    }
}

void Aut::clearParam()
{
    ++gflag;
    #ifndef AUT_NDEBUG
        cout << "[Aut::clearParam] ++gflag=" << gflag << endl;
    #endif
    for (size_t i = 0, size = _itoList.size(); i < size; ++i)
        _itoList[i]->clearParam(0);
    for (size_t i = 0, size = _imdList.size(); i < size; ++i)
        if (_imdList[i]->_flag != gflag) {
            //cout << "[Aut::clearParam] unvisited node=" << _imdList[i]->_name << " " << _imdList[i] << " type=" << _imdList[i]->getTypeStr() << endl;
            _imdList[i]->clearParam(0);
        }
    for (size_t i = 0, size = _predList.size(); i < size; ++i)
        if (_predList[i]->_flag != gflag) {
            _predList[i]->clearParam(0);
        }
}

void Aut::buildParam()
{
    ++gflag;
    #ifndef AUT_NDEBUG
        cout << "[Aut::buildParam] ++gflag=" << gflag << endl;
    #endif
    for (size_t i = 0, size = _itoList.size(); i < size; ++i)
        _itoList[i]->buildParam(0);
    for (size_t i = 0, size = _imdList.size(); i < size; ++i)
        if (_imdList[i]->_flag != gflag) {
            //cout << "[Aut::buildParam] unvisited node=" << _imdList[i]->_name << " " << _imdList[i] << " type=" << _imdList[i]->getTypeStr() << endl;
            _imdList[i]->buildParam(0);
        }
    for (size_t i = 0, size = _predList.size(); i < size; ++i)
        if (_predList[i]->_flag != gflag) {
            _predList[i]->buildParam(0);
        }
}

void Aut::collectPARAM()
{
    ++gflag;
    for (size_t i = 0, size = _itoList.size(); i < size; ++i)
        _itoList[i]->collectPARAM(_PARAMList);
    for (size_t i = 0, size = _imdList.size(); i < size; ++i)
        _imdList[i]->collectPARAM(_PARAMList);
    for (size_t i = 0, size = _predList.size(); i < size; ++i)
        _predList[i]->collectPARAM(_PARAMList);
}

void Aut::renameDef()
{
    #ifndef AUT_PARAM_NDEBUG
        cout << "[Aut::renameDef]\n";
        vector<string> PARAMname;
        vector<string> PARAMsourcebefore;
        for (size_t i = 0, size = _PARAMList.size(); i < size; ++i) {
            PARAMname.push_back(_PARAMList[i]->_name);
            PARAMsourcebefore.push_back(_PARAMList[i]->_source->_name);
        }
    #endif
    int dCnt = -1;
    for (size_t i = 0, size = _imdList.size(); i < size; ++i) {
        assert( (_imdList[i] != epsilon && _imdList[i] != leftAngle && _imdList[i] != rightAngle) );
        _imdList[i]->_name = "d" + itos(++dCnt);
    }
    for (size_t i = 0, size = _predList.size(); i < size; ++i) {
        _predList[i]->_name = "d" + itos(++dCnt);
    }
    clearParam();
    buildParam();
    //_eUsed = (   epsilon->_flag == gflag) ? 1:0;
    //_lUsed = ( leftAngle->_flag == gflag) ? 1:0;
    //_rUsed = (rightAngle->_flag == gflag) ? 1:0;
    #ifndef AUT_PARAM_NDEBUG
        for (size_t i = 0, size = _PARAMList.size(); i < size; ++i) {
            assert( (PARAMname[i] == _PARAMList[i]->_name) );
            cout << PARAMname[i] << " " << _PARAMList[i] << " " << PARAMsourcebefore[i] << " -> " << _PARAMList[i]->_source->_name << endl;
        }
    #endif
}

void Aut::shiftStateVar(const size_t& delta)
{
    while ( (_stateVarNum + delta) > stateBitNum ) expandPIList(STATE);
    _state[0].clear();
    _state[1].clear();
    for (size_t i = delta; i < _stateVarNum + delta; ++i) {
        _state[0].push_back(state[0][i]);
        _state[1].push_back(state[1][i]);
    }
    ++gflag;
    for (size_t i = 0, size = _itoList.size(); i < size; ++i)
        _itoList[i]->shiftStateVar(delta);
    for (size_t i = 0, size = _imdList.size(); i < size; ++i)
        if (_imdList[i]->_flag != gflag) {
            //cout << "[Aut::shiftStateVar] unvisited node=" << _imdList[i]->_name << " " << _imdList[i] << " type=" << _imdList[i]->getTypeStr() << endl;
            _imdList[i]->shiftStateVar(delta);
        }
    check(this);
}

void Aut::defineFun(const string& fname, const string& body, VmtNodeList& list)
{
    #ifndef AUT_NDEBUG
        cout << "[Aut::defineFun] name=" << fname << " body=" << body << endl;
    #endif
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

void Aut::addParamNode(const string& name, VmtNode* source)
{
    assert( (source->_paramList[4].empty()) );
    VmtNode* n = new VmtNode(name,source);
    #ifndef AUT_PARAM_NDEBUG
        _PARAMList.push_back(n);
    #endif
    _vmap.insert(Str2VmtNode(name,n));
    // copy paramList include INPUT,EXIST,STATE,LEN,STATE_N,LEN_N,PREDBV,PREDIV
    n->_paramList = source->_paramList;
    // clear STATE
    n->_paramList[2].clear();
    // move STATE to STATE_N
    for (size_t i = 0, size = source->_paramList[2].size(); i < size; ++i) {
        size_t idx = source->_paramList[2][i]->_idx;
        n->_paramList[4].push_back(state[1][idx]);
    }
}

void Aut::addParamNode(const string& name, VmtNode* source, const size_t& encode, const bool& isExistential)
{
    VmtNode* n = new VmtNode(name,source);
    #ifndef AUT_PARAM_NDEBUG
        _PARAMList.push_back(n);
    #endif
    _vmap.insert(Str2VmtNode(name,n));
    // record INPUT
    VmtNodeList xList = source->_paramList[0];
    // copy paramList include INPUT,EXIST,STATE,LEN,STATE_N,LEN_N,PREDBV,PREDIV
    n->_paramList = source->_paramList;
    // clear INPUT
    n->_paramList[0].clear();

    if (isExistential) {
        // push EXIST to beginning
        VmtNodeList eList;
        const size_t& evbpos = encode;
        for (size_t i = evbpos, size = evbpos + xList.size(); i < size; ++i)
            eList.push_back(evar[0][i]);
        for (size_t i = 0, size = n->_paramList[1].size(); i < size; ++i)
            eList.push_back(n->_paramList[1][i]);
        // substitute paramList[1]
        n->_paramList[1] = eList;
    }
    else {
        string bitstr = Uint2BitString(encode,inputBitNum);
        for (size_t i = 0, size = xList.size(); i < size; ++i) {
            size_t idx = xList[i]->_idx;
            if (bitstr[idx] == '0') n->_paramList[0].push_back(const0);
            else                    n->_paramList[0].push_back(const1);
        }
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
    assert( (_imdList.empty()) );
    assert( (_itoList.empty()) );
    // Make sure _stateVarNum is already handled
    // _evar , _lvar may already be non-empty
    
    // merge evar
    integrateMerge(_evar,evar,a1->_evar[0],a2->_evar[0]);

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
    integrateMerge(_lvar,lvar,a1->_lvar[0],a2->_lvar[0]);

    // merge _predBV _predIV
    integrateMerge(_predBV,predBV,a1->_predBV[0],a2->_predBV[0]);
    integrateMerge(_predIV,predIV,a1->_predIV[0],a2->_predIV[0]);
    
    // merge _imdList
    _imdList = a1->_imdList;
    for (size_t i = 0, size = a2->_imdList.size(); i < size; ++i)
        _imdList.push_back(a2->_imdList[i]);
    
    // merget _PARAMList
    #ifndef AUT_PARAM_NDEBUG
        assert( (_PARAMList.empty()) );
        for (size_t i = 0, size = a1->_PARAMList.size(); i < size; ++i)
            for (size_t j = 0, size1 = a2->_PARAMList.size(); j < size1; ++j)
                assert( (a1->_PARAMList[i] != a2->_PARAMList[j]) );
        _PARAMList = a1->_PARAMList;
        for (size_t i = 0, size = a2->_PARAMList.size(); i < size; ++i)
            _PARAMList.push_back(a2->_PARAMList[i]);
    #endif
}

void Aut::integrateMerge(VarList& varList, const VarList& gVarList, const VmtNodeList& v1, const VmtNodeList& v2)
{
    set<size_t> idxSet;
    for (size_t i = 0, size = v1.size(); i < size; ++i)
        idxSet.insert(v1[i]->_idx);
    for (size_t i = 0, size = v2.size(); i < size; ++i)
        idxSet.insert(v2[i]->_idx);
    for (size_t i = 0, size = varList[0].size(); i < size; ++i)
        idxSet.insert(varList[0][i]->_idx);
    
    varList[0].clear();
    varList[1].clear();

    for (set<size_t>::iterator it = idxSet.begin(); it != idxSet.end(); ++it) {
        varList[0].push_back(gVarList[0][*it]);
        varList[1].push_back(gVarList[1][*it]);
        _vmap.insert(Str2VmtNode(gVarList[0][*it]->_name,gVarList[0][*it]));
        _vmap.insert(Str2VmtNode(gVarList[1][*it]->_name,gVarList[1][*it]));
    }
}

size_t Aut::addEVar(const size_t& size)
{
    size_t evbpos = evarNum;
    while (evbpos + size > evarNum) expandPIList(EXIST);
    for (size_t i = evbpos; i < evbpos + size; ++i) {
        _evar[0].push_back(evar[0][i]);
        _evar[1].push_back(evar[1][i]);
        _vmap.insert(Str2VmtNode(evar[0][i]->_name,evar[0][i]));
    }
    return evbpos;
}

size_t Aut::addStateVar(const size_t& size)
{
    size_t svbpos = _stateVarNum;
    while (svbpos + size > stateBitNum) expandPIList(STATE);
    for (size_t i = _stateVarNum; i < _stateVarNum + size; ++i) {
        _state[0].push_back(state[0][i]);
        _state[1].push_back(state[1][i]);
        _vmap.insert(Str2VmtNode(state[0][i]->_name,state[0][i]));
        _vmap.insert(Str2VmtNode(state[1][i]->_name,state[1][i]));
    }
    _stateVarNum += size;
    return svbpos;
}

void Aut::addLVar(const size_t& lvarIdx)
{
    while (lvarIdx >= lvarNum) expandPIList(LEN);
    // maintain increasing order
    bool isCurPlace = 0, isNxtPlace = 0;
    for (VmtNodeList::iterator it = _lvar[0].begin(); it != _lvar[0].end(); ++it)
        if ((*it)->_idx > lvarIdx) {
            isCurPlace = 1;
            _lvar[0].insert(it,lvar[0][lvarIdx]);
            break;
        }
    for (VmtNodeList::iterator it = _lvar[1].begin(); it != _lvar[1].end(); ++it)
        if ((*it)->_idx > lvarIdx) {
            isNxtPlace = 1;
            _lvar[1].insert(it,lvar[1][lvarIdx]);
            break;
        }
    if (!isCurPlace) _lvar[0].push_back(lvar[0][lvarIdx]);
    if (!isNxtPlace) _lvar[1].push_back(lvar[1][lvarIdx]);

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
    cout << "[Aut::print] itoList" << endl;
    for (size_t i = 0, s = _itoList.size(); i < s; ++i)
        _itoList[i]->print(0);
    cout << "[Aut::print] imdList" << endl;
    for (size_t i = 0, s = _imdList.size(); i < s; ++i)
        _imdList[i]->print(0);
}

void Aut::printPARAMList() const
{
    cout << "[Aut::printPARAMList]\n";
    for (size_t i = 0, size = _PARAMList.size(); i < size; ++i) {
        VmtNode* n = _PARAMList[i];
        assert( (n->_type == PARAM) );
        cout << n->_name << " source=" << n->_source->_name << " params=";
        for (size_t j = 0, size1 = n->_paramList.size(); j < size1; ++j)
            for (size_t k = 0, size2 = n->_paramList[j].size(); k < size2; ++k)
                cout << " " << n->_paramList[j][k]->_name;
        cout << endl;
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
    int sCnt = 0,inputCnt = 0;
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
            cout << "[Aut::parse] line = \"" << line << "\"" << endl;
            cout << "[Aut::parse] tokenList = [ \"" << tokenList[0] << "\" , \"" << tokenList[1] << "\" ]" <<endl;
            cout << "[Aut::parse] paramList = [ ";
            for (size_t i = 0, s = paramList.size(); i < s; ++i) {
                if (i > 0) cout << ", ";
                cout << "\"" << paramList[i] << "\" ";
            }
            cout << endl;
        #endif
        if (tokenList[0] == "epsilon" || tokenList[0] == "leftAngle" || tokenList[0] == "rightAngle") continue;
        if (sCnt == 0) {
            char symbol = tokenList[0][0];
            // only parse current state variable
            if ( *(tokenList[0].rbegin()) == 't' ) continue;
            
            idx = stoi(tokenList[0].substr(1));
            VmtType cType,nType;
            if      ( symbol == 'x' ) { cType = INPUT ; ++inputCnt;}
            else if ( symbol == 's' ) { cType = STATE ; nType = STATE_N; ++_stateVarNum;}
            else if ( symbol == 'n' ) { cType = LEN   ; nType = LEN_N  ; }
            else if ( symbol == 'y' ) { cType = EXIST ; }
            else if ( symbol == 'B' ) { cType = PREDBV; }
            else    { assert( (symbol == 'I') ); cType = PREDIV; }
            
            if ( symbol == 'x') {
                _piList[cType].push_back(piList[ctype][idx]);
                _vmap.insert(Str2VmtNode(piList[cType][idx]->_name,piList[cType][idx]));
            }
            else if ( symbol == 's' || symbol == 'n' ) {
                while ( idx >= piList[cType].size() ) expandPIList(cType);
                _piList[cType].push_back(piList[cType][idx]);
                _piList[nType].push_back(piList[nType][idx]);
                _vmap.insert(Str2VmtNode(piList[cType][idx]->_name,piList[cType][idx]));
                _vmap.insert(Str2VmtNode(piList[nType][idx]->_name,piList[nType][idx]));
            }
            else {
                VmtNode* n = new VmtNode( symbol + itos(idx) , idx );
                _piList[cType].push_back(n);
                _vmap.insert(Str2VmtNode(n->_name,n));
            }
        }
        else {
            // type is handled in the constructor
            VmtNode* module = new VmtNode(tokenList[0]);
            VmtNode* body   = buildVmtNode(tokenList[1],0,tokenList[1].size(),_vmap);
            module->addChild(body);
            for (size_t i = 0, s = paramList.size(); i < s; ++i) {
                Str2VmtNodeMap::iterator jt = _vmap.find(paramList[i]);
                assert( (jt != _vmap.end()) );
                const VmtType& type = jt->second->_type;
                assert( (isPI(type)) );
                module->_paramList[type].push_back(jt->second);
            }
            //cout << "n1=" << module->_name << " n2=" << body->_name << endl;
             _vmap.insert(Str2VmtNode(module->_name,module));
            if      (sCnt == 2) _imdList.push_back(module);
            else if (sCnt == 3) _itoList.push_back(module);
            else if (sCnt == 4) _predList.push_back(module);
        }
    }
    file.close();

    assert( (inputCnt == inputBitNum) );

    // rename local variables and assign to piList for EXIST , PREDBV , PREDIV
    assignGlobalPIList(EXIST);
    assignGlobalPIList(PREDBV);
    assignGlobalPIList(PREDIV);
    
    // rebuild _vmap
    initVMap();
    buildVMap(INPUT);
    buildVMap(STATE);
    buildVMap(STATE_N);
    buildVMap(LEN);
    buildVMap(LEN_N);
    buildVMap(EXIST);
    buildVMap(PREDBV);
    buildVMap(PREDIV);
    buildVMap(_imdList);
    buildVMap(_itoList);
    buildVMap(_predList);
    renameDef();
    #ifndef AUT_PARAM_NDEBUG
        collectPARAM();
        printPARAMList();
    #endif
}

void Aut::write(const string& fileName)
{
    write(fileName.c_str());
}

void Aut::write(const char* fileName)
{
    ofstream file(fileName);
    
    writeDeclareFun( INPUT  , file );
    writeDeclareFun( EXIST  , file );
    writeDeclareFun( STATE  , file );
    writeDeclareFun( LEN    , file );
    writeDeclareFun( STATE_N, file );
    writeDeclareFun( LEN_N  , file );
    writeDeclareFun( PREDBV , file );
    writeDeclareFun( PREDIV , file );
    
    file << ";\n";
    
    int nxtCnt = -1;
    writeNextFun( INPUT , nxtCnt, file );
    writeNextFun( EXIST , nxtCnt, file );
    writeNextFun( STATE , nxtCnt, file );
    writeNextFun( LEN   , nxtCnt, file );
    writeNextFun( PREDBV, nxtCnt, file );
    writeNextFun( PREDIV, nxtCnt, file );
    
    file << ";\n";

    if (epsilon->_flag    == gflag) writeDefineFun( epsilon   , file );
    if (leftAngle->_flag  == gflag) writeDefineFun( leftAngle , file );
    if (rightAngle->_flag == gflag) writeDefineFun( rightAngle, file );

    //if (_eUsed) writeDefineFun( epsilon   , file );
    //if (_lUsed) writeDefineFun( leftAngle , file );
    //if (_rUsed) writeDefineFun( rightAngle, file );

    ++gflag;

    for (size_t i = 0,size = _imdList.size(); i < size; ++i)
        writeDefineFun(_imdList[i],file);
    file << ";\n";
    
    for (size_t i = 0,size = _predList.size(); i < size; ++i)
        writeDefineFun(_predList[i],file);
    if (!_predList.empty()) file << ";\n";

    // ITO cannot be specified their params
    for (size_t i = 0,size = _itoList.size(); i < size; ++i)
        writeDefineFun(_itoList[i],file,0);
    
    file.close();
}

void Aut::writeDeclareFun(const VmtType& type, ofstream& file)
{
    assert( (isPI(type)) );
    string dtype = (type == LEN || type == LEN_N || type == PREDIV) ? "Int" : "Bool";
    for (size_t i = 0, size = _piList[type].size(); i < size; ++i)
        file << "(declare-fun " << _piList[type][i]->_name << " () " << dtype << ")\n" ;
}

void Aut::writeNextFun(const VmtType& type, int& nxtCnt, ofstream& file)
{
    assert( (isCurPI(type)) );
    for (size_t i = 0, size = _piList[type].size(); i < size; ++i)
        file << "(define-fun nxt" << ++nxtCnt << " () Bool " 
             << "(! " << _piList[type][i]->_name << " :next " 
             << _piList[type][i]->_name<< ".next))\n";
}

void Aut::writeDefineFun(VmtNode* n, ofstream& file, const bool& needParam)
{
    #ifndef AUT_NDEBUG
        cout << "[Aut::writeDefineFun] gflag=" << gflag << endl;
        cout << "[Aut::writeDefineFun] " << n->_name << " " << n << " " << n->getTypeStr() << " " << n->_flag << endl;
    #endif
    n->_flag = gflag;
    file << "(define-fun " << n->_name << " (";
    if (needParam) n->writeParamHead(file);
    file << ") Bool ";
    n->_children[0]->write(0,file);
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

    #ifndef AUT_OP_NDEBUG
        cout << "[Aut::addlen] " << _name << " lvar=" << lvarIdxStr << endl;
    #endif

    // Add Length Constraint
    renameITO("tmpT", getT());
    defineFun("tmp1", "(and (not epsilon) (= " + lvn + " (+ " + lv + " 1)))", _imdList);
    defineFun("tmp2", "(and epsilon (= " + lvn + " " + lv + "))", _imdList);
    defineFun("tmp3", "(and tmpT (or tmp1 tmp2))", _imdList, &Aut::setT);
    renameDef();
}

void Aut::intersect(Aut* a1, Aut* a2)
{
    check(a1,a2);
    
    #ifndef AUT_OP_NDEBUG
        cout << "[Aut::intersect] " << a1->_name << " " << a2->_name << endl;
        cout << "[Aut::intersect] origin  svar " << a1->_name << ":";
        for (size_t i = 0, size = a1->_state[0].size(); i < size; ++i)
            cout << " " << a1->_state[0][i]->_name;
        cout << " " << a2->_name << ":";
        for (size_t i = 0, size = a2->_state[0].size(); i < size; ++i)
            cout << " " << a2->_state[0][i]->_name;
    #endif

    // Shift State Variable of a2
    a2->shiftStateVar(a1->_stateVarNum);
    _stateVarNum = a1->_stateVarNum + a2->_stateVarNum;
    
    #ifndef AUT_OP_NDEBUG
        cout << "\n[Aut::intersect] shifted svar " << a1->_name << ":";
        for (size_t i = 0, size = a1->_state[0].size(); i < size; ++i)
            cout << " " << a1->_state[0][i]->_name;
        cout << " " << a2->_name << ":";
        for (size_t i = 0, size = a2->_state[0].size(); i < size; ++i)
            cout << " " << a2->_state[0][i]->_name;
        cout << endl;
    #endif

    // Integrate _state, _lvar, _imdList into this
    integrate(a1,a2);
    
    // Rename ITO
    renameITOs2Aut(a1,a2);

    // Initial
    defineFun( "tmp1", "(and tmpI1 tmpI2)", _imdList);
    defineFun( "I", "(! tmp1 :init true)", _itoList );
    
    // Transition
    defineFun( "T1_epsilon", "(or tmpT1 (and epsilon" + a1->CSNSEquiv(STATE) + a1->CSNSEquiv(LEN) + "))", _imdList);
    defineFun( "T2_epsilon", "(or tmpT2 (and epsilon" + a2->CSNSEquiv(STATE) + a2->CSNSEquiv(LEN) + "))", _imdList);
    defineFun( "tmp2", "(and T1_epsilon T2_epsilon)", _imdList);
    defineFun( "T", "(! tmp2 :trans true)", _itoList);

    // Accepting
    defineFun( "tmp3", "(and tmpO1 tmpO2)", _imdList);
    defineFun( "O", "(! (not tmp3) :invar-property 0)", _itoList);

    renameDef();
}

void Aut::concate(Aut* a1,Aut* a2)
{
    check(a1,a2);
    
    _stateVarNum = (a1->_stateVarNum > a2->_stateVarNum)? a1->_stateVarNum : a2->_stateVarNum;
    size_t svbpos = addStateVar(1);
    string sa  = state[0][svbpos]->_name;
    string san = state[1][svbpos]->_name;

    #ifndef AUT_OP_NDEBUG
        cout << "[Aut:concate] " << a1->_name << " " << a2->_name << " alpha=" << svbpos << endl;
    #endif

    // Integrate _state, _lvar, _imdList into this
    integrate(a1,a2);
    
    // Rename ITO
    renameITOs2Aut(a1,a2);

    // Initial
    defineFun( "tmp1", "(and (not " + sa + ") tmpI1)", _imdList);
    defineFun( "I", "(! tmp1 :init true)", _itoList);
    
    // Transition
    addParamNode( "I2_PARAM", a2->getI());
    defineFun( "tmp2", "(and (not " + sa + ") (not " + san + ") tmpT1" + a2->CSNSEquiv(LEN) + ")", _imdList);
    defineFun( "tmp3", "(and " + sa + " " + san + " tmpT2" + a1->CSNSEquiv(LEN) + ")", _imdList);
    defineFun( "tmp4", "(and epsilon (not " + sa + ") " + san + " tmpO1 I2_PARAM" + CSNSEquiv(LEN) + ")", _imdList);
    defineFun( "tmp5", "(or tmp2 tmp3 tmp4)", _imdList);
    defineFun( "T", "(! tmp5 :trans true)", _itoList);

    // Accepting
    defineFun( "tmp6", "(and " + sa + " tmpO2)", _imdList);
    defineFun( "O", "(! (not tmp6) :invar-property 0)", _itoList);

    renameDef();
}

size_t Aut::mark()
{
    check(this);
    // construct the equivalence before adding svar
    string sveq   = CSNSEquiv(STATE);
    string lveq   = CSNSEquiv(LEN);
    size_t svbpos = addStateVar(1);
    #ifndef AUT_OP_NDEBUG
        cout << "[Aut::mark] " << _name << " alpha=" << svbpos << endl;
    #endif
    string sa  = state[0][svbpos]->_name;
    string san = state[1][svbpos]->_name;
    
    renameITO1Aut();
    
    // Initial State
    defineFun( "tmp1", "(and (not " + sa + ") tmpI1)", _imdList, &Aut::setI );

    // Transition Relation
    defineFun( "tmp2", "(and (= " + sa + " " + san + ") (not " + leftAngle->_name + ") (not " + rightAngle->_name + ") tmpT1)", _imdList);
    defineFun( "tmp3", "(or (and (not " + sa + ") " + san + " " + leftAngle->_name + ") (and " + sa + " (not " + san + ") " + rightAngle->_name + "))", _imdList);
    defineFun( "tmp4", "(and" + sveq + lveq + " tmp3)", _imdList);
    defineFun( "tmp5", "(or tmp2 tmp4)", _imdList, &Aut::setT);

    // Accepting States
    defineFun( "tmp5", "(and (not " + sa + ") tmpO1)", _imdList, &Aut::setO );

    renameDef();
    return svbpos;
}

void Aut::replace_A4(Aut* a1, Aut* a2)
{
    check(a1,a2);
    
    _stateVarNum  = (a1->_stateVarNum > a2->_stateVarNum)? a1->_stateVarNum : a2->_stateVarNum;
    size_t svbpos = addStateVar(1);
    #ifndef AUT_OP_NDEBUG
        cout << "[Aut::replace_A4] " << a1->_name << " " << a2->_name << " beta=" << svbpos << endl;
    #endif
    
    string sa  = state[0][svbpos]->_name;
    string san = state[1][svbpos]->_name;
    
    // Integrate _state, _lvar, _imdList into this
    integrate(a1,a2);
    
    // Rename ITO
    renameITOs2Aut(a1,a2);

    // Initial State
    defineFun( "tmp1", "(and (not " + sa + ") tmpI2)", _imdList);
    defineFun( "I", "(! tmp1 :init true)", _itoList);

    // Transition Relation
    defineFun( "tmp2", "(and (not " + sa + ") (not " + san + ") (not " + leftAngle->_name + ") (not " + rightAngle->_name + ") tmpT2)", _imdList);
    defineFun( "tmp3", "(and " + sa + " " + san + " (not " + leftAngle->_name + ") (not " + rightAngle->_name + ") tmpT1)", _imdList);
    defineFun( "tmp4", "(and (not " + sa + ") " + san + " " + leftAngle->_name + " tmpO2 tmpI1)", _imdList);
    defineFun( "tmp5", "(and " + sa + " (not " + san + ") " + rightAngle->_name + " tmpO1 tmpI2)", _imdList);
    defineFun( "tmp6", "(or tmp2 tmp3 tmp4 tmp5)", _imdList);
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
    
    string csnseq = a1->CSNSEquiv(STATE) + a1->CSNSEquiv(LEN);

    #ifndef AUT_OP_NDEBUG
        cout << "[Aut::replace] " << a1->_name << " " << a2->_name << " alpha=" << alpha << " gamma=" << svbpos << endl;
    #endif
    
    // Integrate _state, _lvar, _imdList into this
    integrate(a1,a2);
    
    // Rename ITO
    renameITOs2Aut(a1,a2);

    // Initial State
    defineFun( "tmp1", "(and (not " + sa1 + ") tmpI1 tmpI2)", _imdList);
    defineFun( "I", "(! tmp1 :init true)", _itoList);
    
    // Transition Relation
    size_t evbpos = addEVar(a1->getT()->_paramList[0].size());
    addParamNode("I2_PARAM",a2->getI());

    addParamNode("T1_PARAM_1",a1->getT(),LEFT_ANGLE_BRACKET_ENCODE,0);
    addParamNode("T1_PARAM_2",a1->getT(),evbpos,1);
    addParamNode("T1_PARAM_3",a1->getT(),RIGHT_ANGLE_BRACKET_ENCODE,0);
    
    defineFun( "tmp2", "(and (not " + sa0 + ") (not " + san0 + ") tmpT1 (not " + sa1 + ") (not " + san1 + ") tmpI2 I2_PARAM)", _imdList);
    defineFun( "tmp3", "(and (not " + sa0 + ") (not " + san0 + ") epsilon" + csnseq + " (not " + sa1 + ") " + san1 + " tmpI2 I2_PARAM)", _imdList);
    defineFun( "tmp4", "(and (not " + sa0 + ") (not " + san0 + ")" + csnseq + " " + sa1 + " " + san1 + " tmpT2)", _imdList);
    defineFun( "tmp5", "(and (not " + sa0 + ") " + san0 + " epsilon T1_PARAM_1 " + sa1 + " (not " + san1 + ") I2_PARAM tmpO2)", _imdList);
    defineFun( "tmp6", "(and " + sa0 + " " + san0 + " epsilon T1_PARAM_2 (not " + sa1 + ") (not " + san1 + ") tmpI2 I2_PARAM)", _imdList);
    defineFun( "tmp7", "(and " + sa0 + " (not " + san0 + ") epsilon T1_PARAM_3 (not " + sa1 + ") (not " + san1 + ") tmpI2 I2_PARAM)", _imdList);
    defineFun( "tmp8", "(or tmp2 tmp3 tmp4 tmp5 tmp6 tmp7)", _imdList);
    defineFun( "T", "(! tmp8 :trans true)", _itoList);
    
    // Accepting States
    defineFun( "tmp9", "(and (not " + sa1 + ") tmpO1 tmpI2)", _imdList);
    defineFun( "O", "(! (not tmp9) :invar-property 0)", _itoList);
    
    renameDef();
}

void Aut::prefix(const string& lvarIdxStr)
{
    size_t evbpos  = addEVar(getT()->_paramList[0].size());
    addParamNode("T1_PARAM",getT(),evbpos,1);
    string csnseq  = CSNSEquiv(STATE) + CSNSEquiv(LEN);
    size_t lvarIdx = stoi(lvarIdxStr);
    check(this,lvarIdx);
    addLVar(lvarIdx);
    size_t svbpos = addStateVar(1);
    string sa  = state[0][svbpos]->_name;
    string san = state[1][svbpos]->_name;
    string lv  = lvar[0][lvarIdx]->_name;
    string lvn = lvar[1][lvarIdx]->_name;

    string lveq    = " (= " + lvn + " " + lv + ")";
    string lvincre = " (= " + lvn + " (+ " + lv + " 1))";

    #ifndef AUT_OP_NDEBUG
        cout << "[Aut::prefix] " << _name << " alpha=" << svbpos << " lvar=" << lvarIdxStr << endl;
    #endif

    renameITO1Aut();

    // Initial State
    defineFun( "tmp1", "(and (not " + sa + ") tmpI1)", _imdList, &Aut::setI );

    // Transition Relation
    defineFun( "T1_epsilon", "(or tmpT1 (and epsilon" + csnseq + "))", _imdList);
    
    defineFun( "tmp2", "(or (and (not epsilon)" + lvincre + ") (and epsilon" + lveq + "))", _imdList);
    defineFun( "tmp3", "(and (not " + sa + ") (not " + san + ") T1_epsilon tmp2)", _imdList);
    defineFun( "tmp4", "(and (not " + sa + ") " + san + " epsilon" + csnseq + lveq + ")", _imdList);
    defineFun( "tmp5", "(and " + sa + " " + san + " epsilon T1_PARAM" + lveq + ")", _imdList);
    defineFun( "tmp6", "(or tmp3 tmp4 tmp5)", _imdList, &Aut::setT );
    
    // Accepting States
    defineFun( "tmp7", "(and " + sa + " tmpO1)", _imdList, &Aut::setO );
    renameDef();
}

void Aut::suffix(const string& lvarIdxStr)
{
    string csnseq  = CSNSEquiv(STATE) + CSNSEquiv(LEN);
    size_t lvarIdx = stoi(lvarIdxStr);
    check(this,lvarIdx);

    size_t evbpos = addEVar(getT()->_paramList[0].size());
    VmtNode* ye = new VmtNode("yEpsilon");
    ye->addChild(new VmtNode("and"));
    for (size_t i = evbpos; i < evbpos + inputBitNum; ++i) {
        VmtNode* notNode = new VmtNode("not");
        notNode->addChild(evar[0][i]);
        ye->_children[0]->addChild(notNode);
    }
    _imdList.push_back(ye);
    _vmap.insert(Str2VmtNode("yEpsilon",ye));

    defineFun( "T1_epsilon", "(or tmpT1 (and epsilon" + csnseq + "))", _imdList);
    addParamNode("T1_PARAM",_imdList.back(),evbpos,1);
    
    addLVar(lvarIdx);
    size_t svbpos = addStateVar(1);
    string sa  = state[0][svbpos]->_name;
    string san = state[1][svbpos]->_name;
    string lv  = lvar[0][lvarIdx]->_name;
    string lvn = lvar[1][lvarIdx]->_name;
    
    string lveq    = " (= " + lvn + " " + lv + ")";
    string lvincre = " (= " + lvn + " (+ 1 " + lv + "))";

    #ifndef AUT_OP_NDEBUG
        cout << "[Aut::suffix] " << _name << " alpha=" << svbpos << " lvar=" << lvarIdxStr << endl;
    #endif
    
    renameITO1Aut();

    // Initial State
    defineFun( "tmp1", "(and (not " + sa + ") tmpI1)", _imdList, &Aut::setI );
    
    // Transition Relation
    defineFun( "tmp2", "(or (and (not yEpsilon)" + lvincre + ") (and yEpsilon" + lveq + "))", _imdList);
    defineFun( "tmp3", "(and (not " + sa + ") (not " + san + ") epsilon T1_PARAM tmp2)", _imdList);
    defineFun( "tmp4", "(and (not " + sa + ") " + san + " epsilon" + csnseq + lveq + ")", _imdList);
    defineFun( "tmp5", "(and " + sa + " " + san + " tmpT1" + lveq + ")", _imdList);
    defineFun( "tmp6", "(or tmp3 tmp4 tmp5)", _imdList, &Aut::setT );
    
    // Accepting States
    defineFun( "tmp7", "(and " + sa + " tmpO1)", _imdList, &Aut::setO );
    
    renameDef();
}

void Aut::substr(const string& n0, const string& n1)
{
    prefix(n1);
    suffix(n0);
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
    // using a stand-alone vmap to prevent naming collision
    Str2VmtNodeMap predVMap;
    while (getline(file,line)) {
        if (line == ";") {
            ++sCnt; continue;
        }
        if      (sCnt == 0) parseDef (line, predVMap);
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
    for (size_t i = predBVNum, size = _predBV[0].size(); i < size; ++i) {
        predBV[0].push_back(_predBV[0][i]);
        predBV[1].push_back(_predBV[1][i]);
    }
    predBVNum = _predBV[0].size();

    for (size_t i = 0, size = _predIV[0].size(); i < size; ++i) {
        string rename = "I" + itos(i);
        _predIV[0][i]->_name = rename;
        _predIV[1][i]->_name = rename + ".next";
    }
    for (size_t i = predIVNum, size = _predIV[0].size(); i < size; ++i) {
        predIV[0].push_back(_predIV[0][i]);
        predIV[1].push_back(_predIV[1][i]);
    }
    predIVNum = _predIV[0].size();

    // and predicates together
    VmtNode* n0   = new VmtNode("O_addpred");
    VmtNode* and0 = new VmtNode("and");
    n0->addChild(and0);
    and0->addChild(getO());
    for (size_t i = 0, size = _predList.size(); i < size; ++i) {
        _imdList.push_back(_predList[i]);
        and0->addChild(_predList[i]);
    }
    _imdList.push_back(n0);
    setO(n0);
    
    // add initial condiditon for length variables (i.e. (= nk 0) )
    VmtNode* n1   = new VmtNode("I_addpred");
    VmtNode* and1 = new VmtNode("and");
    n1->addChild(and1);
    and1->addChild(getI());
    for (size_t i = 0, size = _lvar[0].size(); i < size; ++i) {
        VmtNode* eq = new VmtNode("=");
        eq->addChild(new VmtNode("0"));
        eq->addChild(_lvar[0][i]);
        and1->addChild(eq);
    }
    _imdList.push_back(n1);
    setI(n1);
    renameDef();
}

void Aut::parseDef(const string& line, Str2VmtNodeMap& vmap)
{
    #ifndef AUT_OP_NDEBUG
        cout << "[Aut::parseDef] line = \"" << line << "\"" << endl;
    #endif
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
    
    #ifndef AUT_OP_NDEBUG
        cout << "[Aut::parseDef] name=" << name << " nameNxt=" << nameNxt << " type=" << type << endl;
    #endif

    VmtNode* newNode0 = new VmtNode(name);
    VmtNode* newNode1 = new VmtNode(nameNxt);
    vmap.insert(Str2VmtNode(name,newNode0));
    vmap.insert(Str2VmtNode(nameNxt,newNode1));

    assert( (type == "Bool" || type == "Int") );

    if (type == "Bool") {
        newNode0->_type = PREDBV;
        _predBV[0].push_back(newNode0);
        _predBV[1].push_back(newNode1);
    }
    else {
        newNode0->_type = PREDIV;
        _predIV[0].push_back(newNode0);
        _predIV[1].push_back(newNode1);
    }
}

void Aut::parsePred(const string& line, size_t& pCnt, Str2VmtNodeMap& vmap)
{
    #ifndef AUT_OP_NDEBUG
        cout << "[Aut::parsePred] line = \"" << line << "\"" << endl;
    #endif
    if (line[8] != '(') {
        size_t i = 8;
        while (line[i] != ')') ++i;
        string var = line.substr(8,i-8);
        Str2VmtNodeMap::iterator it = vmap.find(var);
        assert((it != vmap.end()));
        defineFun( "p" + itos(pCnt++), var, _predList, vmap);
    }
    else {
        if ( *(line.rbegin()) == ')') {
            string body = line.substr(8,line.size()-1-8);
            defineFun( "p" + itos(pCnt++), body, _predList, vmap);
        }
        else {
            size_t i = 8;
            size_t dCnt = 1;
            while (dCnt != 0) {
                ++i;
                if     (line[i] == '(') ++dCnt;
                else if(line[i] == ')') --dCnt;
            }
            vector<string> bodyTokens;
            str2tokens(line.substr(8,(i+1)-8)," ()",bodyTokens);

            vector<string> tokenList;
            str2tokens(line.substr(line.find_last_of(';')+1),tokenList);
            
            if (tokenList[0] == "len") {
                assert( (tokenList.size() == 2) );
                assert( (bodyTokens[0] == "=") );
                assert( (bodyTokens[2] == "str.len" || bodyTokens[1] == "str.len") );
                size_t lvarIdx = stoi(tokenList[1]);
                VmtNode* pNode = new VmtNode( "p" + itos(pCnt++) );
                _predList.push_back(pNode);
                vmap.insert(Str2VmtNode(pNode->_name,pNode));

                string aname;
                if (bodyTokens[2] == "str.len") aname = bodyTokens[1];
                else                            aname = bodyTokens[3];

                VmtNode* alias = 0;
                Str2VmtNodeMap::iterator it = vmap.find(aname);
                if (it == vmap.end()) alias = new VmtNode(aname);
                else                  alias = it->second;

                VmtNode* eq = new VmtNode("=");
                eq->addChild(alias);
                eq->addChild(lvar[0][lvarIdx]);
                pNode->addChild(eq);
            }
            else if (tokenList[0] == "substr") {
                assert( (tokenList.size() == 3) );
                assert( (bodyTokens[0] == "str.substr") );
                size_t n0 = stoi(tokenList[1]);
                size_t n1 = stoi(tokenList[2]);
                VmtNode* pNode0 = new VmtNode( "p" + itos(pCnt++) );
                VmtNode* pNode1 = new VmtNode( "p" + itos(pCnt++) );
                _predList.push_back(pNode0);
                _predList.push_back(pNode1);
                vmap.insert(Str2VmtNode(pNode0->_name,pNode0));
                vmap.insert(Str2VmtNode(pNode1->_name,pNode1));

                VmtNode* i1Node = 0;
                VmtNode* i2Node = 0;
                Str2VmtNodeMap::iterator it0  = vmap.find(bodyTokens[2]);
                if (it0 == vmap.end()) i1Node = new VmtNode(bodyTokens[2]);
                else                   i1Node = it0->second;
                Str2VmtNodeMap::iterator it1  = vmap.find(bodyTokens[3]);
                if (it1 == vmap.end()) i2Node = new VmtNode(bodyTokens[3]);
                else                   i2Node = it1->second;

                VmtNode* eq0   = new VmtNode("=");
                VmtNode* plus0 = new VmtNode("+");
                plus0->addChild(i2Node);
                plus0->addChild(new VmtNode("1"));
                eq0->addChild(lvar[0][n0]);
                eq0->addChild(plus0);
                pNode0->addChild(eq0);
                
                VmtNode* eq1   = new VmtNode("=");
                VmtNode* plus1 = new VmtNode("+");
                plus1->addChild(i1Node);
                plus1->addChild(i2Node);
                eq1->addChild(lvar[0][n1]);
                eq1->addChild(plus1);
                pNode1->addChild(eq1);
            }
            else if (tokenList[0] == "indexof") {
                assert( (tokenList.size() == 3) );
                assert( (bodyTokens[0] == "=") );
                assert( (bodyTokens[2] == "str.indexof" || bodyTokens[1] == "str.indexof") );
                size_t n0 = stoi(tokenList[1]);
                size_t n1 = stoi(tokenList[2]);
                VmtNode* pNode0 = new VmtNode( "p" + itos(pCnt++) );
                VmtNode* pNode1 = new VmtNode( "p" + itos(pCnt++) );
                _predList.push_back(pNode0);
                _predList.push_back(pNode1);
                vmap.insert(Str2VmtNode(pNode0->_name,pNode0));
                vmap.insert(Str2VmtNode(pNode1->_name,pNode1));
                
                string aname,kname;
                if (bodyTokens[2] == "str.indexof") {
                    aname = bodyTokens[1];
                    kname = bodyTokens[5];
                }
                else {
                    aname = bodyTokens[5];
                    kname = bodyTokens[4];
                }
                VmtNode* alias = 0;
                VmtNode* kNode = 0;
                Str2VmtNodeMap::iterator it0 = vmap.find(aname);
                assert( (it0 != vmap.end()) );
                alias = it0->second;
                Str2VmtNodeMap::iterator it1 = vmap.find(kname);
                if (it1 == vmap.end()) kNode = new VmtNode(kname);
                else                   kNode = it1->second;
                
                VmtNode* eq0   = new VmtNode("=");
                VmtNode* plus0 = new VmtNode("+");
                plus0->addChild(lvar[0][n0]);
                plus0->addChild(lvar[0][n1]);
                eq0->addChild(alias);
                eq0->addChild(plus0);
                pNode0->addChild(eq0);

                VmtNode* eq1   = new VmtNode("=");
                eq1->addChild(kNode);
                eq1->addChild(lvar[0][n1]);
                pNode1->addChild(eq1);
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
            ret += " (= " + _state[0][i]->_name + " " + _state[1][i]->_name + ")";
    }
    else {
        for (size_t i = 0, size = _lvar[0].size(); i < size; ++i)
            ret += " (= " + _lvar[0][i]->_name + " " + _lvar[1][i]->_name + ")";
    }
    return ret;
}
