#include "autMgr.h"
#include <map>

using namespace std;

extern AutMgr* autmgr;
static size_t& gflag = autmgr->getGFlag();

void VmtNode::print(const size_t& level) const
{
    //cout << string(level*3,' ') << _name << " " << this << " " << Aut::getTypeStr(_type) << " " << _flag;
    cout << string(level*3,' ') << _name << " " << Aut::getTypeStr(_type) << " " << _flag;
    /*
    for (size_t i = 0; i < PI_NUM; ++i) {
        for (size_t j = 0, size = _paramList[i].size(); j < size; ++j)
            cout << " " << _paramList[i][j]->_name;
    }*/
    cout << endl;
    for (size_t i = 0, size = _children.size(); i < size; ++i) {
        _children[i]->print(level+1);
    }
}

void VmtNode::write(const size_t& level,ofstream& outFile) const
{
    cout << string(level*3,' ') << _name << " " << Aut::getTypeStr(_type) << " " << _flag << endl;
    #ifndef VMTNODE_NDEBUG
        cout << string(level*3,' ') << _name << " " << this << " " << Aut::getTypeStr(_type) << " " << _flag;
        if   (_flag == gflag)            cout << " visited";
        else                             cout << " unvisited";
        if   (Aut::isLEAF(_type))        cout << " LEAF\n";
        else {assert(Aut::isIMD(_type)); cout << " IMD\n";}
    #endif
    /*
    if (_flag == gflag) {
        if (_type == PARAM) {
            assert( (_children.empty()) );
            assert( (_source->_flag == gflag) );
            assert( (hasParam()) );
            writeParamBody(_source->_name,outFile);
        }
        else {
            if (!_children.empty()) {
                if ( hasParam() ) {
                    writeParamBody(_name,outFile);
                }
                else {
                    cout << "[VmtNode::write] special node name=" << _name << " c0=" << _children[0]->_name << endl;
                    assert( (_children.size() == 1) );
                    assert( (_children[0] == Aut::const0 || _children[0] == Aut::const1) );
                    outFile << _name;
                }
            }
            else {
                outFile << _name;
            }
        }
        return;
    }
    */
    if ( _flag == gflag ) {
        if ( Aut::isLEAF(_type) ) {
            cout << "name=" << _name << " " << this << " type=" << Aut::getTypeStr(_type) << endl;
            // type=PARAM could be refered multiple times
            assert( (_type != NUM &&  _type != SPECIAL) );
            if ( _type == PARAM ) {
                assert( (_children.empty()) );
                assert( (_source->_flag == gflag) );
                assert( (hasParam()) );
                writeParamBody(_source->_name,outFile);
            }
            else
                outFile << _name;
        }
        else {
            assert( (_type == MODULE) );
            assert( (!_children.empty()) );
            if ( hasParam() ) 
                writeParamBody(_name,outFile);
            else {
                cout << "[VmtNode::write] special node name=" << _name << " c0=" << _children[0]->_name << endl;
                assert( (_children.size() == 1) );
                assert( (_children[0] == Aut::const0 || _children[0] == Aut::const1) );
                outFile << _name;
            }
        }
        return;
    }
    _flag = gflag;
    if ( Aut::isLEAF(_type) ) {
        if (_type == PARAM) {
            assert( (_children.empty()) );
            assert( (_source->_flag == gflag) );
            assert( (hasParam()) );
            writeParamBody(_source->_name,outFile);
        }
        else
            outFile << _name;
    }
    else {
        assert( (Aut::isIMD(_type)) );
        assert( (!_children.empty()) );
        //cout << "name=" << _name << " " << this << " type=" << Aut::getTypeStr(_type) << endl;
        outFile << "(" << _name;
        for (size_t i = 0, size = _children.size(); i < size; ++i) {
            outFile << " ";
            _children[i]->write(level+1,outFile);
        }
        outFile << ")";
    }
    /*
    if (_type == PARAM) {
        assert( (_children.empty()) );
        assert( (_source->_flag == gflag) );
        assert( (hasParam()) );
        writeParamBody(_source->_name,outFile);
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
    */
}

VmtType VmtNode::getType(const string& name) const
{
    if      ( name == "not"         ) return NOT;
    else if ( name == "-"           ) return MINUS; // default all MINUS ; merge NEG for _predList
    else if ( name == "and"         ) return AND;
    else if ( name == "or"          ) return OR;
    else if ( name == "+"           ) return PLUS;
    else if ( name == "<"           ) return LT;
    else if ( name == "<="          ) return LTOEQ;
    else if ( name == "="           ) return EQ;
    else if ( name == ">="          ) return MTOEQ;
    else if ( name == ">"           ) return MT;
    else if ( name == "!"           ) return EXCM;
    else if ( name == "false"       ) return CONST0;
    else if ( name == "true"        ) return CONST1;
    else if ( isNumber(name)        ) return NUM;
    else if ( Aut::isSpecialString(name) ) return SPECIAL;
    else if ( Aut::isPISymbol(name[0]) ) Aut::getPITypeByName(name);
    else                              return MODULE;
}

bool VmtNode::hasParam() const
{
    for (size_t i = 0; i < PI_NUM; ++i) {
        if (!_paramList[i].empty())
            return 1;
    }
    return 0;
}

bool VmtNode::haveSameParam(VmtNode* n) const
{
    for (size_t i = 0; i < PI_NUM; ++i) {
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
        cout << string(level*3,' ') << _name << " " << this << " " << Aut::getTypeStr(_type) << " " << _flag;
        if      (_flag == gflag)     cout << " visited";
        else if (Aut::isLEAF(_type)) cout << " LEAF";
        cout << endl;
    #endif
    if ( _flag == gflag || Aut::isLEAF(_type) ) return;
    
    _flag = gflag;
    
    for (size_t i = 0; i < PI_NUM; ++i)
        _paramList[i].clear();
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->clearParam(level+1);
}

void VmtNode::buildParam(const size_t& level)
{
    #ifndef VMTNODE_NDEBUG
        cout << string(level*3,' ') << _name << " " << this << " " << Aut::getTypeStr(_type) << " " << _flag;
        if      (_flag == gflag)     cout << " visited";
        else if (Aut::isLEAF(_type)) cout << " LEAF";
        cout << endl;
    #endif
    if ( _flag == gflag || Aut::isLEAF(_type) ) return;
    _flag = gflag;

    // do not push CONST0 CONST1 into _paramList
    vector<set<size_t> > count(PI_NUM,set<size_t>());
    for (size_t i = 0, size = _children.size(); i < size; ++i) {
        _children[i]->buildParam(level+1);
        const VmtType& type = _children[i]->_type;
        if ( Aut::isPI(type) ) {
            count[type].insert( _children[i]->_idx );
        }
        else if ( Aut::isIMD(type) || type == PARAM ){
            for (size_t j = 0; j < PI_NUM; ++j) {
                for (size_t k = 0; k < _children[i]->_paramList[j].size(); ++k) {
                    if (_children[i]->_paramList[j][k]->_type != CONST0 &&
                        _children[i]->_paramList[j][k]->_type != CONST1   )
                        count[j].insert( _children[i]->_paramList[j][k]->_idx );
                }
            }
        }
    }
    // push parameters into _paramList increasingly
    for (size_t i = 0; i < PI_NUM; ++i) {
        for (set<size_t>::iterator it = count[i].begin(); it != count[i].end(); ++it) {
            _paramList[i].push_back(Aut::piList[i][*it]);
        }
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

void VmtNode::mergeEquivalence()
{
    if ( _flag == gflag ) return;
    
    _flag = gflag;
    if ( _type == MINUS && _children.size() == 1) _type = NEG;

    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->mergeEquivalence();
    
    if ( _type == PLUS ) {
        assert( (_children.size() == 2) );
        const VmtType& t0 = _children[0]->_type;
        const VmtType& t1 = _children[1]->_type;
        if ( t1 == MINUS || t0 == MINUS ) {
            if ( t0 == MINUS ) {
                // swap
                VmtNode* tmp = _children[0];
                _children[0] = _children[1];
                _children[1] = tmp;
            }
            assert( (_children[1]->_children.size() == 1) );
            const VmtType& t10 = _children[1]->_children[0]->_type;
            assert( (Aut::isPREDINT(t10)) );
            // merge
            _children[1] = _children[1]->_children[0];
            _type = MINUS;
        }
    }
    else if ( _type == MINUS ) {
        assert( (_children.size() == 2) );
        const VmtType& t0 = _children[0]->_type;
        const VmtType& t1 = _children[1]->_type;
        if ( t1 == MINUS ) {
            assert( (_children[1]->_children.size() == 1) );
            const VmtType& t10 = _children[1]->_children[0]->_type;
            assert( (Aut::isPREDINT(t10)) );
            // merge
            _children[1] = _children[1]->_children[0];
            _type = PLUS;
        }
    }
    else if ( _type == MT || _type == MTOEQ ) {
        assert( (Aut::isPREDINT(_children[0]->_type)) );
        assert( (Aut::isPREDINT(_children[1]->_type)) );
        if ( _type == MT ) _type = LT;
        else               _type = LTOEQ;
        // swap
        VmtNode* tmp = _children[0];
        _children[0] = _children[1];
        _children[1] = tmp;
    }
}

void VmtNode::writeParamHead(ofstream& file) const
{
    cout << "name=" << _name << " type" << Aut::getTypeStr(_type) << endl;
    assert( (_type == MODULE) );
    bool isfirst = 1;
    #ifndef VMTNODE_NDEBUG
        cout << "[VmtNode::writeParamHead]";
    #endif
    for (size_t i = 0; i < PI_NUM; ++i) {
        for (size_t j = 0, size = _paramList[i].size(); j < size; ++j) {
            #ifndef VMTNODE_NDEBUG
                cout << " " << _paramList[i][j]->_name 
                     << " " << Aut::getTypeStr(_paramList[i][j]->_type);
            #endif
            if (!isfirst) file << " ";
            isfirst = 0;
            file << "(" << _paramList[i][j]->_name;
            if (i == LEN || i == LEN_N || i == PREDIV) {
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

void VmtNode::writeParamBody(const string& fname, ofstream& file) const
{
    file << "(" << fname;
    for (size_t i = 0; i < PI_NUM; ++i) {
        for (size_t j = 0, size = _paramList[i].size(); j < size; ++j)
            file << " " << _paramList[i][j]->_name;
    }
    file << ")";
}

void VmtNode::shiftStateVar(const size_t& delta)
{
    if ( _flag == gflag || (_type != PARAM && !Aut::isIMD(_type)) ) return;
    
    _flag = gflag;
    if (_type == PARAM) {
        assert((_children.empty()));
        
        // change STATE 
        VmtNodeList tmp;
        for (size_t i = 0, size = _paramList[STATE].size(); i < size; ++i)
            tmp.push_back( Aut::piList[STATE][ _paramList[STATE][i]->_idx + delta ] );
        _paramList[STATE] = tmp;
        
        // change STATE_N
        tmp.clear();
        for (size_t i = 0, size = _paramList[STATE_N].size(); i < size; ++i)
            tmp.push_back( Aut::piList[STATE_N][ _paramList[STATE_N][i]->_idx + delta ] );
        _paramList[STATE_N] = tmp;
        return;
    }

    for (size_t i = 0, size = _children.size(); i < size; ++i) {
        _children[i]->shiftStateVar(delta);
        //cout << "node = " << _name << " children[" << i << "] = " << _children[i]->_name << endl;
        const VmtType& type = _children[i]->_type;
        if (type != STATE && type != STATE_N) continue;
        _children[i] = Aut::piList[type][ _children[i]->_idx + delta ];
    }
}

void VmtNode::buildBLIF(int& tCnt)
{
    if ( _flag == gflag ) return;
    _flag = gflag;
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->buildBLIF(tCnt);
    if ( Aut::isPI(_type) || _type == NUM || _type == MODULE ) _bname = _name;
    else if ( Aut::isIMD(_type) || _type == PARAM ) _bname = "t" + itos(++tCnt);
}

void VmtNode::writeMODEL(ofstream& file, const size_t& intBitNum, vector<size_t>& minSize, vector<size_t>& maxSize)
{
    file << ".model " << _bname << "\n.inputs";
    for (size_t i = 0; i < PI_NUM; ++i)
        for (size_t j = 0, size = _paramList[i].size(); j < size; ++j)
            file << " " << _paramList[i][j]->_name;
    file << "\n.outputs " << _bname << "_0\n";
    bool tUsed = 0, fUsed = 0;
    writeSUBCKT(file,intBitNum,minSize,maxSize,tUsed,fUsed);
    if (tUsed) file << ".names const1\n1\n";
    if (fUsed) file << ".names const0\n";
    file << ".end\n";
}

void VmtNode::writeSUBCKT(ofstream& file, const size_t& intBitNum, vector<size_t>& minSize, vector<size_t>& maxSize, bool& tUsed, bool& fUsed)
{
    /*
    for ( size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->writeSUBCKT(file,intBitNum,minSize,maxSize,tUsed,fUsed);
    
    if ( Aut::isINT(_type) ) {
        // LEN LEN_N PREDIV NUM
        assert( ( ~Aut::isPREDINT(_type) || intBitNum == INPUT_ENCODE_BIT_NUM + 1) );
        _bit = intBitNum;
    }
    else if ( _type == PARAM ) {
        assert( (_source->_type == MODULE) );
        file << ".subckt " << _source->_bname;
        for (size_t i = 0; i < PI_NUM; ++i) {
            assert( (_paramList[i]->size() == _source->_paramList[i]->size()) );
            for (size_t j = 0, size = _paramList[i]->size(); j < size; ++j) {
                file << " " << _source->_paramList[i][j]->_bname << "=" << _paramList[i][j]->_bname << endl;
            }
        }
        file << " " << _source->_bname << "_0=" << _bname << "_0" << endl;
    }
    else if ( _type == NOT) {
        assert( (_children.size() == 1) );
        file << ".names " << _children[0]->_bname << "_0 " << _bname << "_0";
             << "\n0 1\n";
    }
    else if ( _type == NEG ) {
        assert( (_children.size() == 1) );
        const VmtType& type = _children[0]->_type;
        const size_t&  bit  = _children[0]->_bit;
        assert( (Aut::isINT(type) && type != LEN_N) );
        _bit  = bit + 1;
        tUsed = 1;
        fUsed = 1;
        file << ".subckt " << bit << "bAINV";
        for (size_t i = bit - 1; i >= 0; --i)
            file << " a" << i << "=const0";
        if ( Aut::isINT(_type) ) {
        }
        for (size_t i = bit - 1; i >= 0; --i) {
            if 
            file << " b" << i << "=x" << i;
        }
        file << " sign=const1";
        for (size_t i = bit - 1; i >= 0; --i)
            file << " s" << i << "=y" << i;
        file << " cout=y" << bit << endl;
    }
    else if ( _type == AND ) {
        file << ".names";
        for (size_t i = 0, size = _children.size(); i < size; ++i)
            file << " " << _children[i]->_bname;
        file << " out\n";
        for (size_t i = 0, size = _children.size(); i < size; ++i)
            file << "1";
        file << " 1\n";
    }
    else if ( _type == OR ) {
        file << ".names";
        for (size_t i = 0, size = _children.size(); i < size; ++i)
            file << " " << _children[i]->_bname;
        file << " out\n";
        for (size_t i = 0, size = _children.size(); i < size; ++i) {
            string bitstr(size,'-'); 
            bitstr[i] = '1';
            file << bitstr << " 1" << endl;
        }
    }
    else if ( _type == PLUS) {
        assert( (_children.size() == 2) );
        const VmtType& type0 = _children[0]->_type;
        const VmtType& type1 = _children[1]->_type;
        assert( ());
    }
    else if ( _type == MINUS) {
    }
    else if ( _type == EQ ) {
    }
    else if ( _type == LT ) {
    }
    else {
        assert( (_type == LTOEQ) );
    }
    for ( size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->writeSUBCKT(file,intBitNum,minSize,maxSize,tUsed,fUsed);
    */
}
