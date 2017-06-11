#include "autMgr.h"

void TGEdge::print() const 
{
    cout << _sIdx << " -> "<< _eIdx << " ";
    if (_labels.size() == 2)
        cout << _labels[0] << " - " << _labels[1] << endl;
    else {
        assert( (_labels.size() == 1) );
        cout << _labels[0] << endl;
    }
}

void TGEdge::write(const CubeMap& downCubeMap, const CubeMap& rangeCubeMap, const vector<string>& stateBitStringList, ofstream& file)
{
    file << "\n# " << _sIdx << " -> "<< _eIdx << " ";
    if (_labels.size() == 2)
        file << _labels[0] << " - " << _labels[1] << endl;
    else {
        assert( (_labels.size() == 1) );
        file << _labels[0] << endl;
    }
    _tBitString = stateBitStringList[_sIdx] + stateBitStringList[_eIdx] + " 1\n";
    if (_labels.size() == 2) {
        if (_labels[0] == 0) {
            if (_labels[1] == 65535) {
                file << "0" << downCubeMap.find(65535)->second << _tBitString;
                writeExtraBitSpace(rangeCubeMap,file);
            }
            else {
                CubeMap::const_iterator it = downCubeMap.lower_bound(_labels[1]);
                if (it->first == _labels[1])
                    file << "0" << it->second << _tBitString;
                else {
                    assert( (_labels[1] > 0) );
                    assert( (it->first > _labels[1]) );
                    file << "0" << (--it)->second << _tBitString;
                    writeRangeMinterm("0",it->first+1,_labels[1],file);
                }
            }
        }
        else {
            if (_labels[1] == 65535) {
                CubeMap::const_iterator it = rangeCubeMap.lower_bound(_labels[0]);
                if (it->first/2 + 1 == _labels[0]) {
                    for (; it != rangeCubeMap.end(); ++it)
                        file << "0" << it->second << _tBitString;
                }
                else {
                    writeRangeMinterm("0",_labels[0],it->first,file);
                    for (++it; it != rangeCubeMap.end(); ++it)
                        file << "0" << it->second << _tBitString;
                }
                writeExtraBitSpace(rangeCubeMap,file);
            }
            else {
                CubeMap::const_iterator it0 = rangeCubeMap.lower_bound(_labels[0]);
                CubeMap::const_iterator it1 = rangeCubeMap.lower_bound(_labels[1]);
                if (it0->first/2 + 1 != _labels[0]) {
                    writeRangeMinterm("0",_labels[0],it0->first,file);
                    ++it0;
                }
                if (it1->first == _labels[1]) ++it1;
                else {
                    writeRangeMinterm("0",it1->first/2+1,_labels[1],file);
                }
                for (; it0 != it1; ++it0)
                    file << "0" << it0->second << _tBitString;
            }
        }
    }
    else {
        assert( _labels.size() == 1 );
        file << "0" << Uint2BitString(_labels[0],INPUT_BIT_NUM) << _tBitString;
    }
}

void TGEdge::writeExtraBitSpace(const CubeMap& rangeCubeMap, ofstream& file)
{
    CubeMap::const_iterator it = rangeCubeMap.lower_bound(MAX_SPECIAL_ALPHABET_ENCODE);
    if (it->first == MAX_SPECIAL_ALPHABET_ENCODE) {
        for (++it; it != rangeCubeMap.end(); ++it)
            file << "1" << it->second << _tBitString;
    }
    else {
        assert( (it->first > MAX_SPECIAL_ALPHABET_ENCODE) );
        for (size_t i = MAX_SPECIAL_ALPHABET_ENCODE+1; i <= it->first; ++i)
            file << "1" << Uint2BitString(i,INPUT_BIT_NUM) << _tBitString;
        for (++it; it != rangeCubeMap.end(); ++it)
            file << "1" << it->second << _tBitString;
    }
}

void TGEdge::writeRangeMinterm(const string& extraBit, const size_t& m1, const size_t& m2, ofstream& file)
{
    for (size_t i = m1; i <= m2; ++i)
        file << extraBit << Uint2BitString(i,INPUT_BIT_NUM) << _tBitString;
}

void TGraph::init()
{
    cout << "inputBitNum     = " << INPUT_BIT_NUM << endl
         << "initStateBitNum = " << INIT_STATE_BIT_NUM << endl
         << "initLvarNum     = " << INIT_LVAR_NUM << endl
         << "epsilon         = " << EPSILON_ENCODE << endl
         << "left            = " << LEFT_ANGLE_BRACKET_ENCODE << endl
         << "right           = " << RIGHT_ANGLE_BRACKET_ENCODE << endl;
    _numbers.insert('0');
    _numbers.insert('1');
    _numbers.insert('2');
    _numbers.insert('3');
    _numbers.insert('4');
    _numbers.insert('5');
    _numbers.insert('6');
    _numbers.insert('7');
    _numbers.insert('8');
    _numbers.insert('9');
    
    _h2dMap.insert(pair<char,size_t>('0',0));
    _h2dMap.insert(pair<char,size_t>('1',1));
    _h2dMap.insert(pair<char,size_t>('2',2));
    _h2dMap.insert(pair<char,size_t>('3',3));
    _h2dMap.insert(pair<char,size_t>('4',4));
    _h2dMap.insert(pair<char,size_t>('5',5));
    _h2dMap.insert(pair<char,size_t>('6',6));
    _h2dMap.insert(pair<char,size_t>('7',7));
    _h2dMap.insert(pair<char,size_t>('8',8));
    _h2dMap.insert(pair<char,size_t>('9',9));
    _h2dMap.insert(pair<char,size_t>('a',10));
    _h2dMap.insert(pair<char,size_t>('b',11));
    _h2dMap.insert(pair<char,size_t>('c',12));
    _h2dMap.insert(pair<char,size_t>('d',13));
    _h2dMap.insert(pair<char,size_t>('e',14));
    _h2dMap.insert(pair<char,size_t>('f',15));

    size_t prod = 2;
    string bitstr (INPUT_BIT_NUM,'0');
    for (int i = 15; i >= 0; --i) {
        bitstr[i] = '-';
        _downCubeMap.insert(pair<size_t,string>(prod-1,bitstr));
        prod *= 2;
    }

    bitstr = string(INPUT_BIT_NUM,'0');
    bitstr[15] = '-';
    _rangeCubeMap.insert(pair<size_t,string>(1,bitstr));
    prod = 4;
    for (int i = 14; i >= 0; --i) {
        bitstr[i+1] = '-';
        bitstr[i]   = '1';
        _rangeCubeMap.insert(pair<size_t,string>(prod-1,bitstr));
        prod *= 2;
    }
}

void TGraph::parse(const char* fileName)
{
    string line;
    ifstream file(fileName);

    if (!file) {
        cout << "fail open file=" << fileName << endl;
        return;
    }
    size_t maxStateIdx = 0;
    set<size_t> oSet;
    vector<string> tokenList;
    while (getline(file,line)) {
        tokenList.clear();
        str2tokens(line,tokenList);
        /*cout << "line=" << line << endl;
        for (size_t i = 0, size = tokenList.size(); i < size; ++i)
            cout << tokenList[i] << " ";
        cout << endl;*/
        if (tokenList[0] == "initial" && tokenList[1] == "->")
            _initStateIdx = stoi(tokenList[2]);
        else if (_numbers.find(tokenList[0][0]) != _numbers.end()) {
            if (tokenList[1] == "->") {
                size_t sIdx = stoi(tokenList[0]);
                size_t eIdx = stoi(tokenList[2]);
                TGEdge* edge = new TGEdge(sIdx,eIdx);
                parseLabels(tokenList[3], edge->_labels);
                _tList.push_back(edge);
            }
            else {
                assert( (   tokenList[1][0] == '[' && 
                        ( *(tokenList[1].rbegin()) == ']'||
                          *(tokenList[1].rbegin()) == ';') ));
                size_t idx = stoi(tokenList[0]);
                if (idx > maxStateIdx) maxStateIdx = idx;
                if (isAccepting(tokenList[1])) oSet.insert(idx);
            }
        }
        
    }
    file.close();
    
    _stateBitNum = 0;
    size_t tmp = maxStateIdx;
    while (tmp % 2 != 0 || tmp / 2 != 0) {
        ++_stateBitNum;
        tmp /= 2;
    }

    for (size_t i = 0; i <= maxStateIdx; ++i)
        _stateBitStringList.push_back(Uint2BitString(i,_stateBitNum));

    for (set<size_t>::iterator it = oSet.begin(); it != oSet.end(); ++it)
        _oList.push_back(*it);
    
}

void TGraph::write(const char* fileName)
{
    write(string(fileName));
}

void TGraph::write(const string& fileName)
{
    ofstream file(fileName.c_str());

    file << ".model " << fileName.substr(0,fileName.find_last_of('.'))
         << "\n.inputs";
    for (size_t i = 0; i < INPUT_BIT_NUM + 1; ++i) file << " x" << i;
    for (size_t i = 0; i < _stateBitNum; ++i) file << " s" << i;
    for (size_t i = 0; i < _stateBitNum; ++i) file << " n" << i;
    file << "\n.outputs i o t";

    // initial state
    file << "\n.names";
    for (size_t i = 0; i < _stateBitNum; ++i) file << " s" << i;
    file << " i\n" << _stateBitStringList[_initStateIdx] << " 1\n";

    // accpeting states
    file << "\n.names";
    for (size_t i = 0; i < _stateBitNum; ++i) file << " s" << i;
    file << " o\n";
    for (size_t i = 0, size = _oList.size(); i < size; ++i)
        file << _stateBitStringList[_oList[i]] << " 1\n";
    // transition function
    file << "\n.names";
    for (size_t i = 0; i < INPUT_BIT_NUM + 1; ++i) file << " x" << i;
    for (size_t i = 0; i < _stateBitNum; ++i) file << " s" << i;
    for (size_t i = 0; i < _stateBitNum; ++i) file << " n" << i;
    file << " t\n";
    for (size_t i = 0, size = _tList.size(); i < size; ++i) 
        _tList[i]->write(_downCubeMap,_rangeCubeMap,_stateBitStringList,file);
    
    file << "\n.end";
    file.close();
}

void TGraph::print() const
{
    cout << "initial\n" << _initStateIdx << endl;
    cout << "accpeting\n";
    for (size_t i = 0, size = _oList.size(); i < size; ++i)
        cout << _oList[i] << endl;
    cout << "transition\n";
    for (size_t i = 0, size = _tList.size(); i < size; ++i)
        _tList[i]->print();
}

bool TGraph::isAccepting(const string& line)
{
    size_t i = 0;
    while(line[i] != '=') ++i;
    size_t j = ++i;
    while(line[i] != ',') ++i;
    string type = line.substr(j,i-j);
    assert( (type == "circle" || type == "doublecircle") );
    if (type == "circle") return 0;
    else                  return 1;
}

void TGraph::parseLabels(const string& line, vector<size_t>& labels)
{
    size_t i = 0;
    while (line[i] != '\"') ++i;
    size_t j = ++i;
    bool isRange = 1;
    while (line[i] != '-') {
        if (line[i] == '\"') {
            isRange = 0;
            break;
        }
        ++i;
    }
    if (isRange) {
        labels.push_back(label2UTF16(line.substr(j,i-j)));
        j = ++i;
        while (line[i] != '\"') ++i;
        labels.push_back(label2UTF16(line.substr(j,i-j)));
    }
    else {
        labels.push_back(label2UTF16(line.substr(j,i-j)));
    }
}

//TODO
size_t TGraph::label2UTF16(const string& s)
{
    if (s[0] == '\\') {
        size_t prod = 1, sum = 0;
        for (size_t i = 5; i >= 2; --i) {
            sum += prod * _h2dMap[s[i]];
            prod *= 16;
        }
        return sum;
    }
    else {
        assert( (s.size() == 1) );
        return size_t(s[0]);
    }
}
